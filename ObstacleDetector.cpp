/*
 * ObstacleDetector.cpp
 *
 *  Created on: Sep 12, 2012
 *      Author: rghunter
 */

#include "ObstacleDetector.h"


ObstacleDetector::ObstacleDetector(double hessian_threshold, int octaves, float pattern_size, const char *camera_calibration)
{
	detector = new cv::SurfFeatureDetector(hessian_threshold,octaves);
	grid_detector = new cv::GridAdaptedFeatureDetector(detector,100,GRID_SIZE,GRID_SIZE);
	extractor = new cv::FREAK(true,true,pattern_size,octaves);
	refresh = false;

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
	delete(detector);
	delete(grid_detector);
	delete(extractor);
}

float ObstacleDetector::euclid_distance(cv::KeyPoint pt1, cv::KeyPoint pt2)
{
	return (float)sqrt(pow((double)(pt1.pt.x - pt2.pt.x),2)+pow((double)(pt1.pt.y - pt2.pt.y),2));
}
void ObstacleDetector::getTrainPoints(vector<cv::KeyPoint> *train_points)
{
	train_points->clear();
	for(unsigned int i=0;i<keypoint_train.size();i++)
		train_points->push_back(keypoint_train[i]);
}
void ObstacleDetector::getQueryPoints(vector<cv::KeyPoint> *query_points)
{

	query_points->clear();
	for(unsigned int i=0;i<keypoint_query.size();i++)
		query_points->push_back(keypoint_query[i]);
}
void ObstacleDetector::getMatches(vector<cv::DMatch> *inMatch){
	inMatch->clear();
	for(unsigned int i=0;i<matches.size();i++)
		inMatch->push_back(matches[i]);
}
void ObstacleDetector::matchFrame(cv::Mat input_image)
{
	this->undistort(input_image);

	detector->detect(undistorted_frame,keypoint_query);
	extractor->compute(undistorted_frame,keypoint_query,descriptor_query);
	if(keypoint_query.size() == 0)
		return;
	if(keypoint_train.size() == 0 || refresh)
		this->initialFrame(undistorted_frame,keypoint_query,descriptor_query);
	matcher.match(descriptor_train,descriptor_query,matches); //note: query and train are swapped on purpose
	for(unsigned int i=0;i<matches.size();i++)
		if(matches[i].distance > ACC_HAMMING){
			matches.erase(matches.begin()+i);
			if(++matchRate[i] > 4)
				refresh = true;
		}
}
void ObstacleDetector::undistort(cv::Mat raw_frame)
{
	if(undistortImage){
		cv::undistort(raw_frame,undistorted_frame,camera_matrix,distortion_coeffs);
	}else{
		raw_frame.copyTo(undistorted_frame);
	}
}
void ObstacleDetector::initialFrame(cv::Mat input_image, vector<cv::KeyPoint> detected_keypoints, cv::Mat descriptors)
{
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
				if((distances.at<float>(row,col) < (float)90) && keypoint_mask.at<char>(row,1)){
					int badPoint = averages[row] > averages[col] ? row : col;
					detected_keypoints.erase(detected_keypoints.begin()+badPoint);
					keypoint_mask.at<char>(badPoint,1) = 0;
				}
	extractor->compute(input_image,detected_keypoints,descriptors);
	cv::drawKeypoints(undistorted_frame,keypoint_train,initial_frame,cv::Scalar(255,255,0));
	matchRate.clear();
	descriptors.copyTo(descriptor_train);
	keypoint_train.clear();
	for(unsigned int i=0;i<detected_keypoints.size();i++)
		keypoint_train.push_back(detected_keypoints[i]);
	for(unsigned int i=0;i<keypoint_train.size();i++)
		matchRate.push_back(0);
	refresh = false;
}


