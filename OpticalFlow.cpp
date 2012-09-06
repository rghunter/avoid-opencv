#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <stdlib.h>
#include <fstream>

#define ACC_ERROR 4

using namespace cv;
using namespace std;

Mat outgoing_frame, frame_gray, save_image, fixed_image;
Mat frame_corrected;
Mat fixed_descriptor;
vector<KeyPoint> keypoint_fixed;

float distance(KeyPoint *pt1, KeyPoint *pt2)
{
	//float x = pow(pt1->x,2);
	//float y = pt1->y;
	//return x+y;
	return sqrt(pow(((float)pt1->pt.x - pt2->pt.x),2)+pow((pt1->pt.y - pt2->pt.y),2));
}


int main(void)
{
	//VideoCapture video("./static.mp4");
	VideoCapture video(0);


	SurfFeatureDetector detector(4000,1);
	FREAK extractor(true,true,70,1);
	BruteForceMatcher <Hamming> matcher;
	vector<KeyPoint> keypoint;
	Mat descriptor;
	Mat raw_frame;
	Mat input;
	Mat output_frame;
	//vector<vector<DMatch> > matches;
	vector<DMatch> matches;
	int first = 10;
	ofstream data_log0, data_log1;
	data_log0.setf(ios::floatfield);
	data_log0.precision(5);
	data_log0.open("./hitMissEval.csv");

	vector<vector<char> > MASK;

	//MASK.zeros(100,100,CV_8U);
	//MASK.col(1) = 1;

	data_log0 << "Hit, Miss, Total" << endl;

	//double frame_count = video.get(CV_CAP_PROP_FRAME_COUNT);
	int current_count = 0;

	cout << "Begin.... Please Wait..." << endl;

	while((current_count < 100)) //|| (frame_count - video.get(CV_CAP_PROP_POS_FRAMES)) > 1)
	{
		video >> input;
		current_count += 1;
		if(true)
		{
			//raw_frame = input.clone();
			cvtColor(input,raw_frame,CV_BGR2GRAY);
			if(first > 0){
				first--;
				continue;
			}
			if(first == 0)
			{
				fixed_image = raw_frame.clone();
				detector.detect(raw_frame,keypoint_fixed);
				extractor.compute(raw_frame,keypoint_fixed,fixed_descriptor);
				first = -1;
				continue;
			}

			detector.detect(raw_frame,keypoint);
			extractor.compute(raw_frame,keypoint,descriptor);
			matcher.match(descriptor,fixed_descriptor,matches);
		//	matcher.radiusMatch(descriptor, fixed_descriptor,matches,1,Mat(),true);
			float sum = 0;
			float len = 0;
			vector<char> MASK;
			float correct, incorrect;
			correct = 0;
			incorrect = 0;
			for(uint i=0;i<matches.size();i++)
			{

				KeyPoint pt1 = keypoint_fixed[matches[i].trainIdx];
				KeyPoint pt2 = keypoint[matches[i].queryIdx];

				float euclidian_distance = distance(&keypoint_fixed[matches[i].trainIdx],&keypoint[matches[i].queryIdx]);
				cout << euclidian_distance << "\t";

				if(euclidian_distance < ACC_ERROR)
					correct += 1;
				else
					incorrect += 1;
#if 0
				if(matches[i].distance > 5)
					MASK.push_back(0);
				else
				{
					len += 1;
					sum += distance(&keypoint_fixed[matches[i].trainIdx],&keypoint[matches[i].queryIdx]);
					//cout << " " << distance(&keypoint_fixed[matches[i].trainIdx],&keypoint[matches[i].queryIdx]) << " ";
					MASK.push_back(1);
				}
#endif
			}

			data_log0 << correct << ", " << incorrect << ", " << matches.size() <<  ", " << keypoint_fixed.size()<< endl;


		//	cout << "Average Distance: " << (float)(sum/len) << endl;
		//	cout << endl;

			//drawMatches(fixed_image,keypoint_fixed,raw_frame,keypoint,matches,output_frame,Scalar::all(-1),Scalar::all(-1),MASK);
			//imshow("output",output_frame);
			//cout << "Num Points: " << keypoint.size() << " Num Matches: " << matches.size() << endl;
		}
		//waitKey(1);
	}
	cout << "Done!" << endl;
	data_log0.close();
	video.release();
	return 0;
}
