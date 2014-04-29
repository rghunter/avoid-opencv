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

/**
 * \brief	Track features between frame pairs
 *
 * This is the feature tracker used by COLA. We can select different algorithms for use in the tracker by assigning an instance to the
 * relevant pointer in the constructor. It is assumes that all descriptors inherit the relevant interfaces from OpenCV.
 */
class FeatureTracker {

private:
	unsigned int maxFeatures;
	cv::FeatureDetector* detector;
	cv::DescriptorExtractor* descriptorExtractor;
	cv::BruteForceMatcher<cv::Hamming> matcher;
	std::vector<cv::KeyPoint> tempPoints; //we use this as a buffer in case we get more keypoints than we wanted.
	inline float timeElapsed(timespec start, timespec end);

public:

	/**
	 * @brief	FeatureTracker Constructor, initializes the image algorithms and sets max features to track.
	 * We use max features as a means of constraining the computational complexity of the algorithm. FeatureTracker will save the first n
	 * features until maxFeatures is reached.
	 *
	 * @param maxFeatures integer representation of the max features to track
	 */
	FeatureTracker(unsigned int maxFeatures=1000);

	/**
	 * @brief	Destructor
	 */
	virtual ~FeatureTracker();

	/**
	 * @brief	Generate the descriptors for a given frame
	 *
	 * @param &frameDescriptor COLA::FrameDescriptor the query frame to generate descriptors for
	 */
	bool generateDescriptors(COLA::FrameDescriptor &frameDescriptor);

	/**
	 * @brief	frameMatcher creates a COLA::FlowField for a pair of COLA::FrameDescriptors
	 *
	 * @param &train COLA::FrameDescriptor the set of train features
	 * @param &query COLA::FrameDescriptor the set of query features
	 * @param &field COLA::FlowField the data structure we store the flow field in (the difference between tracked points in train -> query)
	 */
	bool frameMatcher(COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field);

};

} /* namespace COLA */

#endif /* FEATURETRACKER_H_ */
