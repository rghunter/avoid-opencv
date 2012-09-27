#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "ObstacleDetector.h"

#define ACC_EUCLID 60

using namespace cv;
using namespace std;

Mat raw_frame, grayscale_frame, initial_image, output_frame;
vector<KeyPoint> keypoints_train, keypoints_query;
vector<DMatch> matches;

float euclid_distance(KeyPoint pt1, KeyPoint pt2)
{
	return (float)sqrt(pow((double)(pt1.pt.x - pt2.pt.x),2)+pow((double)(pt1.pt.y - pt2.pt.y),2));
}

int main(void)
{
	ObstacleDetector detector(3000,4,60,"./calib.xml");

	VideoCapture video("./2.avi");
	//video.set(CV_CAP_PROP_FRAME_WIDTH,300);
	//video.set(CV_CAP_PROP_FRAME_HEIGHT,300);
	//VideoCapture video(0);
	if(!video.isOpened()){
		cout << "ERROR: Could not open file." << endl;
		return 1;
	}

	ofstream data_log0, data_log1;
	data_log0.setf(ios::floatfield);
	data_log0.precision(2);
	data_log0.open("./hitMissEval.csv");
	data_log0 << "Hit, Miss, Total" << endl;

	bool first = true;
	double frame_count = video.get(CV_CAP_PROP_FRAME_COUNT);
	if(frame_count == 0)
		frame_count = 3000;
	int current_count = 0;

	cout << "Number of frames: " << frame_count <<endl;
	cout << "Begin.... Please Wait..." << endl;

	while((frame_count - video.get(CV_CAP_PROP_POS_FRAMES) > 1))
	{
		video >> raw_frame;
		current_count += 1;
		if(current_count > 30)
		{
			cvtColor(raw_frame,grayscale_frame,CV_BGR2GRAY);

			detector.matchFrame(grayscale_frame);
			detector.getMatches(&matches);
			detector.getQueryPoints(&keypoints_query);
			detector.getTrainPoints(&keypoints_train);
			vector<char> MASK_G,MASK_B;
			float correct, incorrect;
			correct = 0;
			incorrect = 0;
			for(uint i=0;i<matches.size();i++)
			{
				KeyPoint pt1 = keypoints_train[matches[i].queryIdx];
				KeyPoint pt2 = keypoints_query[matches[i].trainIdx];

				float euclidian_distance = euclid_distance(pt1,pt2);

				if(euclidian_distance < ACC_EUCLID){
					MASK_G.push_back(1);
					MASK_B.push_back(0);
					correct += 1;
				}else{
					MASK_G.push_back(0);
					MASK_B.push_back(1);
					incorrect += 1;
				}
			}

			data_log0 << correct << ", " << incorrect << ", " << matches.size() <<  ", " << keypoints_train.size()<< endl;

			drawMatches(detector.initial_frame,keypoints_train,grayscale_frame,keypoints_query,matches,output_frame,Scalar(0,255,0),Scalar(0,0,255),MASK_G,DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
			drawMatches(detector.initial_frame,keypoints_train,grayscale_frame,keypoints_query,matches,output_frame,Scalar(0,0,255),Scalar(255,0,0),MASK_B,(DrawMatchesFlags::DRAW_OVER_OUTIMG | DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS));
			imshow("output",output_frame);
		}
		waitKey(10);
	}
	cout << "Done!" << endl;
	data_log0.close();
	video.release();
	return 0;
}
