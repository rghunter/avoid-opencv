/*
 * ObstacleDetector.cpp
 *
 *  Created on: Sep 12, 2012
 *      Author: rghunter
 */

#include "ObstacleDetector.h"

#define REFRESH_AFTER			1



ObstacleDetector::ObstacleDetector(double hessian_threshold, int octaves, float pattern_size, const char *camera_calibration, int height, int width)
{
	FAST_Detector = new cv::FastFeatureDetector(10,true);
	extractor = new cv::FREAK(false,true,40,4);

	frame_count = REFRESH_AFTER+1;

	roi_rec = new cv::Rect(0,height/3,width,height/3);

	nodal_pt = cv::KeyPoint(width/2.0f,height/2.0f,0);

	data_log2.open("./data/performance_data.csv");
	tau_log.open("./data/tau_value.csv");

	cv::FileStorage fs(camera_calibration,cv::FileStorage::READ);
	if(!fs.isOpened()){
		cout << "ERROR: Could not open camera calibration file. No distortion correction will be applied" << endl;
		undistortImage = false;
	}else{
		cout << "Calibration file loaded from: " << camera_calibration << endl;
		fs["Camera_Matrix"] >> camera_matrix;
		fs["Distortion_Coefficients"] >> distortion_coeffs;
		fs.release();
		if(camera_matrix.empty() || distortion_coeffs.empty())
		{
			cout << "ERROR: Calibration file corrupt or incomplete. No distortion correction will be applied" << endl;
			undistortImage = false;
		}else{
			cout << "Calibration data loaded." << endl;
			undistortImage = true;
		}
	}
}

ObstacleDetector::~ObstacleDetector() {
	cout << "Closing Obstacle Detector." << endl;
	data_log2.close();
	tau_log.close();
	delete(FAST_Detector);
	delete(extractor);
}

float ObstacleDetector::euclid_distance(cv::KeyPoint pt1, cv::KeyPoint pt2)
{
	return (float)sqrt(pow((double)(pt1.pt.x - pt2.pt.x),2)+pow((double)(pt1.pt.y - pt2.pt.y),2));
}
void ObstacleDetector::matchFrame(cv::Mat input_image)
{
	timespec begin, end;
	memcpy(&frame_last,&frame_current,sizeof(timespec));
	keypoint_query.clear();
	matches.clear();
	keypoint_query_adjusted.clear();
	tauVals.clear();
	clock_gettime(CLOCK_MONOTONIC,&frame_current);
	cv::Mat newImage;
	clock_gettime(CLOCK_MONOTONIC,&begin);
	cvtColor(input_image,newImage,CV_BGR2GRAY,1);
	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log2 << time_elapsed(begin,end) << ",";
	clock_gettime(CLOCK_MONOTONIC,&begin);
	this->undistort(newImage);
	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log2 << time_elapsed(begin,end) << ",";
	clock_gettime(CLOCK_MONOTONIC,&begin);
	FAST_Detector->detect(roi_subframe,keypoint_query);
	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log2 << time_elapsed(begin,end) << ",";
	clock_gettime(CLOCK_MONOTONIC,&begin);
	extractor->compute(undistorted_frame,keypoint_query,descriptor_query);
	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log2 << time_elapsed(begin,end) << ",";
	clock_gettime(CLOCK_MONOTONIC,&begin);
	if(keypoint_query.size() == 0)
		return;
	keypoint_query_adjusted.clear();
	for(unsigned int i=0;i<keypoint_query.size();i++)
	{
		cv::KeyPoint adjusted = keypoint_query[i];
		adjusted.pt.x += roi_rec->x;
		adjusted.pt.y += roi_rec->y;
		keypoint_query_adjusted.push_back(adjusted);
	}
	if(keypoint_train.size() == 0 || frame_count > REFRESH_AFTER)
	{
		this->initialFrame(roi_subframe,keypoint_query,descriptor_query);
		return;
	}
	frame_count++;
	clock_gettime(CLOCK_MONOTONIC,&begin);
	matcher.match(descriptor_train,descriptor_query,matches); //note: query and train are swapped on purpose
	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log2 << time_elapsed(begin,end) << ",";
	for(unsigned int i=0;i<matches.size();i++)
		if(matches[i].distance > ACC_HAMMING)
			matches.erase(matches.begin()+i);

	for(unsigned int i=0;i<matches.size();i++)
	{
		 cv::Point2f traverse = keypoint_query[matches[i].queryIdx].pt-keypoint_train[matches[i].trainIdx].pt;
		 cv::Point2f nodal_dist = nodal_pt.pt-keypoint_query[matches[i].queryIdx].pt;
		 float traverse_time = time_elapsed(frame_last,frame_current);
		 float tauX = fabs(nodal_dist.x) / (traverse.x / traverse_time);
		 float tauY = fabs(nodal_dist.y) / (traverse.y / traverse_time);
		 cv::KeyPoint curr_loc =  keypoint_query[matches[i].queryIdx];
		 OpticalQuad::Tau tau(cv::Point2f(tauX,tauY),curr_loc);
		 tauVals.push_back(tau);
	}
	tau_log << endl;
	clock_gettime(CLOCK_MONOTONIC,&end);
	data_log2 << time_elapsed(begin,end) << endl;
}
void ObstacleDetector::undistort(cv::Mat raw_frame)
{
	if(undistortImage){
		cv::undistort(raw_frame,undistorted_frame,camera_matrix,distortion_coeffs);
		cv::Mat roi(undistorted_frame,*roi_rec);
		roi.copyTo(roi_subframe);
	}else{
		cv::Mat roi(raw_frame,*roi_rec);
		roi.copyTo(roi_subframe);
		raw_frame.copyTo(undistorted_frame);
	}
}
void ObstacleDetector::initialFrame(cv::Mat input_image, vector<cv::KeyPoint> detected_keypoints, cv::Mat descriptors)
{
	frame_count = 0;

	cv::Mat mask(detected_keypoints.size(),detected_keypoints.size(),CV_8UC1);
	vector<int> averages;
	cv::Mat keypoint_mask = cv::Mat::ones(detected_keypoints.size(),1,CV_8UC1);
	cv::bitwise_not((cv::Mat::eye(detected_keypoints.size(),detected_keypoints.size(),CV_8UC1)*255),mask);
	int normType = cv::NORM_HAMMING;
	cv::Mat distances(detected_keypoints.size(),detected_keypoints.size(),CV_32FC1);
	 int dtype = normType == cv::NORM_HAMMING || normType == cv::NORM_HAMMING2 ||
	        (normType == cv::NORM_L1 && descriptors.type() == CV_8U) ? CV_32S : CV_32S;
	cv::batchDistance(descriptors,descriptors,distances,dtype,cv::noArray(),normType,0,cv::Mat());

	for(unsigned int i = 0;i<detected_keypoints.size();i++)
		averages.push_back(cv::mean(distances.row(i),mask.row(i))[0]);

	for(int col=0;col<(int)detected_keypoints.size();col++)
		if(keypoint_mask.at<char>(col,1))
			for(int row=0;row<(int)detected_keypoints.size();row++)
				if((distances.at<float>(row,col) < (float)20) && keypoint_mask.at<char>(row,1))
				{
					int badPoint = averages[row] > averages[col] ? row : col;
					detected_keypoints.erase(detected_keypoints.begin()+badPoint);
					keypoint_mask.at<char>(badPoint,1) = 0;
				}

	extractor->compute(undistorted_frame,detected_keypoints,descriptors);
	cv::drawKeypoints(undistorted_frame,keypoint_train_adjusted,initial_frame,cv::Scalar(255,255,0));
	descriptors.copyTo(descriptor_train);
	keypoint_train.clear();
	keypoint_train_adjusted.clear();
	for(unsigned int i=0;i<detected_keypoints.size();i++)
	{
		keypoint_train.push_back(detected_keypoints[i]);
		cv::KeyPoint adjusted = detected_keypoints[i];
		adjusted.pt.x += roi_rec->x;
		adjusted.pt.y += roi_rec->y;
		keypoint_train_adjusted.push_back(adjusted);
	}
}
float ObstacleDetector::time_elapsed(timespec start, timespec end)
{
	return (end.tv_sec + end.tv_nsec/1000000000.0) - (start.tv_sec + start.tv_nsec/1000000000.0);
}


