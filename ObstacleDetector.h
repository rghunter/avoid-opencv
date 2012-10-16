/*
 * ObstacleDetector.h
 *
 *  Created on: Sep 12, 2012
 *      Author: rghunter
 */

#ifndef OBSTICALDETECTOR_H_
#define OBSTICALDETECTOR_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sys/time.h>
#include <unistd.h>
#include "Tau.h"

using namespace std;

#define DEFAULT_HESSIAN			2500
#define DEFAULT_OCTAVES			4
#define DEFAULT_PATTERN_SIZE	30

#define GRID_SIZE				10

#define ACC_HAMMING				1

class ObstacleDetector {
public:
	vector<cv::KeyPoint> keypoint_train, keypoint_query,keypoint_train_adjusted,keypoint_query_adjusted;
	vector<cv::DMatch> matches;
	cv::Mat undistorted_frame;

	ObstacleDetector(double hessian_threshold, int octaves, float pattern_size,
			const char *camera_calibration, int height, int width);
	virtual ~ObstacleDetector();
	void matchFrame(cv::Mat input_image);
	void initialFrame(cv::Mat input_image, vector<cv::KeyPoint> detected_keypoints, cv::Mat descriptors);
	cv::Mat initial_frame;
	const cv::Rect *roi_rec;

	vector<OpticalQuad::Tau> tauVals;

private:
	cv::FastFeatureDetector *FAST_Detector;
	cv::FREAK *extractor;
	cv::BruteForceMatcher <cv::Hamming> matcher;
	cv::KeyPoint nodal_pt;
	cv::Mat descriptor_query, descriptor_train;

	cv::Mat camera_matrix, distortion_coeffs;

	float time_elapsed(timespec start, timespec end);

	ofstream data_log2, tau_log;
	cv::Mat roi_subframe;

	timespec frame_last,frame_current;

	int frame_count;

	bool undistortImage;

	float euclid_distance(cv::KeyPoint pt1, cv::KeyPoint pt2);
	void undistort(cv::Mat raw_frame);

};

#endif /* OBSTICALDETECTOR_H_ */
