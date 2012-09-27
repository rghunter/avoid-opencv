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

using namespace std;

#define DEFAULT_HESSIAN			2500
#define DEFAULT_OCTAVES			4
#define DEFAULT_PATTERN_SIZE	30

#define GRID_SIZE				10

#define ACC_HAMMING				5

class ObstacleDetector {
public:
	ObstacleDetector(double hessian_threshold, int octaves, float pattern_size, const char *camera_calibration = "./calib.xml");
	virtual ~ObstacleDetector();
	void getTrainPoints(vector<cv::KeyPoint> *train_points);
	void getQueryPoints(vector<cv::KeyPoint> *query_points);
	void getMatches(vector<cv::DMatch> *inMatch);
	void matchFrame(cv::Mat input_image);
	void initialFrame(cv::Mat input_image, vector<cv::KeyPoint> detected_keypoints, cv::Mat descriptors);
	cv::Mat initial_frame;

private:
	cv::SurfFeatureDetector *detector;
	cv::GridAdaptedFeatureDetector *grid_detector;
	cv::FREAK *extractor;
	cv::BruteForceMatcher <cv::Hamming> matcher;
	vector<cv::KeyPoint> keypoint_train, keypoint_query;
	vector<cv::DMatch> matches;
	cv::Mat descriptor_query, descriptor_train;
	cv::Mat undistorted_frame;
	cv::Mat camera_matrix, distortion_coeffs;

	vector<char> matchRate;

	bool undistortImage, refresh;

	float euclid_distance(cv::KeyPoint pt1, cv::KeyPoint pt2);
	void undistort(cv::Mat raw_frame);
};

#endif /* OBSTICALDETECTOR_H_ */
