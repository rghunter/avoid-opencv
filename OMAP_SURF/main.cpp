/*
 * main.cpp
 *
 *  Created on: Oct 7, 2012
 *      Author: rghunter
 */


#include "SURF.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;
using namespace cv;

int main(void)
{
	VideoCapture video("/video/croppedTest2.mp4");

	OpticalQuad::SURF detector(1,4,4,0.0005f);
	Mat raw_frame, output_image, grayscale_frame;

	vector<KeyPoint> train_points;

	while(video.grab())
	{
		video >> raw_frame;

		cvtColor(raw_frame,grayscale_frame,CV_BGR2GRAY,1);
		detector.ExtractPoints(grayscale_frame,train_points);
		drawKeypoints(raw_frame,train_points,output_image, Scalar(255,0,0));

		imshow("Output",output_image);

		waitKey(1);
	}

	return 0;
}
