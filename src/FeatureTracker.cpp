/*
 * FeatureTracker.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "COLA/FeatureTracker.h"


namespace COLA {

FeatureTracker::FeatureTracker(unsigned int maxFeatures) : maxFeatures(maxFeatures) {

	//cv::Ptr<cv::FeatureDetector> detector_algorithim(new cv::GoodFeaturesToTrackDetector(4,0.001,5));
	cv::Ptr<cv::FeatureDetector> detector_algorithim(new cv::FastFeatureDetector(10,true));
	detector = new cv::GridAdaptedFeatureDetector(detector_algorithim,maxFeatures,2,4);
	descriptorExtractor = new cv::FREAK(false,true,30,4);

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

	COLA::Time &time = *COLA::Time::Instance();

	float time_delta = time.timeElapsed(train,query);

	cv::Point2f original_location;
	cv::Point2f current_location;

	for (unsigned int i=0; i<field.matches.size();i++) {
		if(field.matches[i].size() > 0) {
			original_location = train.featurePoints[field.matches[i][0].queryIdx].pt;
			current_location = query.featurePoints[field.matches[i][0].trainIdx].pt;
			COLA::FlowPoint point(original_location,current_location, time_delta);
			field.push_back(point);
		}
	}

	return true;
}



} /* namespace COLA */
