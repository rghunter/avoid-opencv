/*
 * ObstacleDetector.h
 *
 *  Created on: Sep 12, 2012
 *      Author: rghunter
 */

#ifndef OBSTICALDETECTOR_H_
#define FEATURETRACKER_H

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


namespace COLA {
	class FTrackerOLD {
	public:
		vector<cv::KeyPoint> keypoint_train, keypoint_query, keypoint_query_roi;
		vector<cv::DMatch> matches, matchesUnfiltered;
		cv::Mat undistorted_frame;

		FTrackerOLD(int height, int width);
		virtual ~FTrackerOLD();
		void matchFrame(cv::Mat input_image);
		void initialFrame(cv::Mat input_image, vector<cv::KeyPoint> detected_keypoints, cv::Mat descriptors);
		cv::Mat initial_frame;
		const cv::Rect *roi_rec;

		vector<COLA::Tau> tauVals;

	private:
		cv::FastFeatureDetector *FAST_Detector;
		cv::FREAK *extractor;
		cv::BruteForceMatcher <cv::Hamming> matcher;
		cv::KeyPoint nodal_pt;

		cv::Mat descriptor_query, descriptor_train;
		cv::Mat camera_matrix, distortion_coeffs;


		ofstream data_log2, tau_log;
		cv::Mat roi_subframe;

		timespec frame_last,frame_current;

		int frame_count;

		bool undistortImage;

		float euclid_distance(cv::Point2f pt1, cv::Point2f pt2);
		void undistort(cv::Mat raw_frame);
		float time_elapsed(timespec start, timespec end);
	};
}

#endif
