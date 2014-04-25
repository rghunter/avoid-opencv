/*
 * FeatureTracker.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "COLA/FeatureTracker.h"


namespace COLA {

FeatureTracker::FeatureTracker(unsigned int maxFeatures) : maxFeatures(maxFeatures) {

	detector = new cv::FastFeatureDetector(70,true);
	descriptorExtractor = new cv::FREAK(false,true,10,4);

	tempPoints.reserve(maxFeatures); //preallocate the temp buffer.

}

FeatureTracker::~FeatureTracker() {
	delete detector;
	delete descriptorExtractor;
}

bool FeatureTracker::generateDescriptors(COLA::FrameDescriptor &frameDescriptor) {

	//Need to convert to greyscale
	if(frameDescriptor.roi_set){
		*frameDescriptor.process_frame = cv::Mat(frameDescriptor.refFrame,frameDescriptor.roi_rect);
	}
	cv::cvtColor(*frameDescriptor.process_frame,*frameDescriptor.process_frame,CV_BGR2GRAY,1);

	detector->detect(*frameDescriptor.process_frame,frameDescriptor.featurePoints);

	if (frameDescriptor.featurePoints.size() == 0)
		return false; //we didn't detect anything !!!


	COLA::Time &time = *COLA::Time::Instance();
	time.setTime(frameDescriptor); //Set the timestamp

	//TODO: Perhaps there is a more effient way to write below? Perhaps we can just limit the features in the Detector?
	if(frameDescriptor.featurePoints.size() > maxFeatures)
		frameDescriptor.featurePoints.resize(maxFeatures);

	descriptorExtractor->compute(*frameDescriptor.process_frame,frameDescriptor.featurePoints,frameDescriptor.descriptors); //Now extract the descriptors from the frame
	frameDescriptor.normalizeKeypoints();

	return true; //all is good!
}

bool FeatureTracker::frameMatcher(COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field) {

	field.reset();

	matcher.radiusMatch(train.descriptors,query.descriptors,field.matches,20);

	if(field.matches.size() == 0) {
		return false; //we didn't get any matches
	}

	//TODO: Perhaps we need to filter out matches based on an acceptable maximum HAMMING DISTANCE?

	//Put it all into a flow field

	COLA::Time &time = *COLA::Time::Instance();

	float time_delta = time.timeElapsed(train,query);

	cv::Point2f original_location;
	cv::Point2f current_location;

	for (unsigned int i=0; i<field.matches.size();i++) {
		if(field.matches[i].size() > 0) {
			original_location = train.featurePoints[field.matches[i][0].queryIdx].pt;
			current_location = query.featurePoints[field.matches[i][0].trainIdx].pt;

			cv::Vec2f diff_vector = current_location-original_location;
			diff_vector *= 1.0/time_delta;

			field.push_back(COLA::FlowPoint(current_location, diff_vector));
		}
	}

	return true;
}



} /* namespace COLA */
