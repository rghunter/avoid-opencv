/*
 * ObstacleDetector.cpp
 *
 *  Created on: Sep 12, 2012
 *      Author: rghunter
 */

#include "ObstacleDetector.h"

#define REFRESH_AFTER			0
#define ACC_HAMMING				60



ObstacleDetector::ObstacleDetector(double hessian_threshold, int octaves, float pattern_size, const char *camera_calibration, int height, int width)
{
	FAST_Detector = new cv::FastFeatureDetector(30,true);
	SURF_Detector = new OpticalQuad::SURF(4,4,4,0.0007f);
	extractor = new cv::FREAK(false,true,70,4);

	roi_rec = new cv::Rect(0,height/3,width,height/3);

	nodal_pt = cv::KeyPoint (width/2.0f,height/2.0f,0);

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

float ObstacleDetector::euclid_distance(cv::Point2f pt1, cv::Point2f pt2)
{
	return (float)sqrt(pow((double)(pt1.x - pt2.x),2)+pow((double)(pt1.y - pt2.y),2));
}
void ObstacleDetector::matchFrame(cv::Mat input_image)
{
	timespec begin, end;

	memcpy(&frame_last,&frame_current,sizeof(timespec));
	clock_gettime(CLOCK_MONOTONIC,&frame_current);

	keypoint_query.clear();
	keypoint_query_roi.clear();
	matches.clear();



	cv::Mat newImage;

	cvtColor(input_image,newImage,CV_BGR2GRAY,1);

	this->undistort(newImage);

	FAST_Detector->detect(roi_subframe,keypoint_query_roi);
	//SURF_Detector->ExtractPoints(roi_subframe,keypoint_query_roi);



	for(unsigned int i=0;i<keypoint_query_roi.size();i++)
	{
		cv::KeyPoint adjusted = keypoint_query_roi[i];
		adjusted.pt.x += roi_rec->x;
		adjusted.pt.y += roi_rec->y;
		keypoint_query.push_back(adjusted);
	}

	if(keypoint_query.size() == 0)
		return;

	extractor->compute(undistorted_frame,keypoint_query,descriptor_query);

	if(keypoint_train.size() == 0 || frame_count > REFRESH_AFTER)
	{
		this->initialFrame(undistorted_frame,keypoint_query,descriptor_query);
		return;
	}

	frame_count++;

	matcher.match(descriptor_train,descriptor_query,matchesUnfiltered); //note: query and train are swapped on purpose

#if 1
	for(unsigned int i=0;i<matchesUnfiltered.size();i++)
		if(matchesUnfiltered[i].distance < ACC_HAMMING)
			matches.push_back(matchesUnfiltered[i]);
#endif

	tauVals.clear();

	for(unsigned int i=0;i<matches.size();i++)
	{
		 float origin_pt = euclid_distance(keypoint_train[matches[i].queryIdx].pt,nodal_pt.pt);
		 float current_pt = euclid_distance(keypoint_query[matches[i].trainIdx].pt,nodal_pt.pt);

		 float traverse_time= time_elapsed(frame_last,frame_current);

		 float tauV = (current_pt - origin_pt);/// traverse_time;

		 cv::KeyPoint curr_loc =  keypoint_query[matches[i].trainIdx];
		 OpticalQuad::Tau tau(tauV,curr_loc);
		  tauVals.push_back(tau);

		 tau_log << tauV << ", ";

	}

	tau_log << endl;
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
#if 0
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
				if((distances.at<float>(row,col) < (float)5) && keypoint_mask.at<char>(row,1))
				{
					int badPoint = averages[row] > averages[col] ? row : col;
					detected_keypoints.erase(detected_keypoints.begin()+badPoint);
					keypoint_mask.at<char>(badPoint,1) = 0;
				}
#endif
//	extractor->compute(input_image,detected_keypoints,descriptors);
	//cv::drawKeypoints(undistorted_frame,keypoint_train_adjusted,initial_frame,cv::Scalar(255,255,0));
	input_image.copyTo(initial_frame);
	descriptors.copyTo(descriptor_train);
	keypoint_train.clear();
	for(unsigned int i=0;i<detected_keypoints.size();i++)
		keypoint_train.push_back(detected_keypoints[i]);
}
float ObstacleDetector::time_elapsed(timespec start, timespec end)
{
	return (end.tv_sec + end.tv_nsec/1000000000.0) - (start.tv_sec + start.tv_nsec/1000000000.0);
}


