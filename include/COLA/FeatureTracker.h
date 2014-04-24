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

#include <COLA/FrameDescriptor.h>
#include <COLA/FlowField.h>
#include <COLA/Time.h>

#include <sys/time.h>

namespace COLA {

class FeatureTracker {

private:
	unsigned int maxFeatures;
	cv::FeatureDetector* detector;
	cv::DescriptorExtractor* descriptorExtractor;
	cv::BruteForceMatcher<cv::Hamming> matcher;
	std::vector<cv::KeyPoint> tempPoints; //we use this as a buffer in case we get more keypoints than we wanted.
	inline float timeElapsed(timespec start, timespec end);

public:
	FeatureTracker(unsigned int maxFeatures=1000);
	virtual ~FeatureTracker();
	bool generateDescriptors(COLA::FrameDescriptor &frameDescriptor);
	bool frameMatcher(COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field);
};

} /* namespace COLA */

#endif /* FEATURETRACKER_H_ */
