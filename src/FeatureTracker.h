/*
 * FeatureTracker.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#ifndef FEATURETRACKER_H_
#define FEATURETRACKER_H_

#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "FrameDescriptor.h"
#include "FlowField.h"

#include <sys/time.h>


namespace COLA {

class FeatureTracker : cv::DescriptorMatcher {
private:
	unsigned int maxFeatures;
	cv::Mat greyFrame;

	cv::FeatureDetector* detector;
	cv::DescriptorExtractor* descriptorExtractor;

	std::vector<cv::KeyPoint> tempPoints; //we use this as a buffer in case we get more keypoints than we wanted.

	inline float timeElapsed(timespec start, timespec end);
public:

	FeatureTracker(unsigned int maxFeatures=1000);
	virtual ~FeatureTracker();
	bool generateDescriptors(cv::Mat &frame, COLA::FrameDescriptor &frameDescriptor);
	bool frameMatcher(COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field);
};

} /* namespace COLA */

#endif /* FEATURETRACKER_H_ */
