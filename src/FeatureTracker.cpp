/*
 * FeatureTracker.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "FeatureTracker.h"


namespace COLA {

FeatureTracker::FeatureTracker(unsigned int maxFeatures) : maxFeatures(maxFeatures) {

	detector = new cv::FastFeatureDetector(60,true);
	descriptorExtractor = new cv::FREAK(true,true,70,4);

	tempPoints.reserve(maxFeatures); //preallocate the temp buffer.

}

FeatureTracker::~FeatureTracker() {
	delete detector;
	delete descriptorExtractor;
}

bool FeatureTracker::generateDescriptors(cv::Mat &frame, COLA::FrameDescriptor &frameDescriptor) {

	//Need to convert to greyscale
	cv::cvtColor(frame,greyFrame,CV_BGR2GRAY,1);

	detector->detect(greyFrame,frameDescriptor.featurePoints);

	if (frameDescriptor.featurePoints.size() == 0)
		return false; //we didn't detect anything !!!

	clock_gettime(CLOCK_MONOTONIC, &frameDescriptor.timestamp); //Set the timestamp

	//TODO: Perhaps there is a more effient way to write below? Perhaps we can just limit the features in the Detector?
	if(frameDescriptor.featurePoints.size() > maxFeatures)
		frameDescriptor.featurePoints.resize(maxFeatures);

	descriptorExtractor->compute(greyFrame,frameDescriptor.featurePoints,frameDescriptor.descriptors); //Now extract the descriptors from the frame

	return true; //all is good!
}

bool FeatureTracker::frameMatcher(COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field) {

	field.clear();

	radiusMatch(train.descriptors,query.descriptors,field.matches,60);

	if(field.matches.size() == 0) {
		return false; //we didn't get any matches
	}

	//TODO: Perhaps we need to filter out matches based on an acceptable maximum HAMMING DISTANCE?

	//Put it all into a flow field

	field.timeDelta_sec = timeElapsed(train.timestamp,query.timestamp);

	cv::Point2f original_location;
	cv::Point2f current_location;

#if 0

	for (unsigned int i=0; i<field.matches.size();i++) {
		original_location = train.featurePoints[field.matches[i].queryIdx].pt;
		current_location = query.featurePoints[field.matches[i].trainIdx].pt;

		field.flowField.push_back(COLA::FlowPoint(current_location, current_location-original_location));
	}
#endif

	return true;
}

inline float FeatureTracker::timeElapsed(timespec start, timespec end) {
	return (end.tv_sec + end.tv_nsec/1000000000.0) - (start.tv_sec + start.tv_nsec/1000000000.0);
}

} /* namespace COLA */
