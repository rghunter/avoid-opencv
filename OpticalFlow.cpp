#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <stdlib.h>
#include <fstream>

using namespace cv;
using namespace std;

Mat outgoing_frame, frame_gray, save_image, fixed_image;
Mat frame_corrected;
Mat fixed_descriptor;
vector<KeyPoint> keypoint_fixed;

int main(void)
{

	SurfFeatureDetector detector(5000,4);
	VideoCapture video("./croppedTest2.mp4");
	//video.set(CV_CAP_PROP_FRAME_WIDTH,640);
	//video.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	//GridAdaptedFeatureDetector detector(new SurfFeatureDetector(2000,4), 100, 4,4);
	FREAK extractor(true,true,30,2);
	BruteForceMatcher <HammingLUT> matcher;
	vector<KeyPoint> keypoint;
	Mat descriptor;
	Mat raw_frame;
	Mat input;
	Mat output_frame;
	vector<vector<DMatch> > matches;
	//vector<DMatch> matches;
	bool first = true;
	ofstream data_log;
	data_log.open("./distance.csv");

	while(video.grab())
	{
		video >> input;
		if(!input.empty())
		{
			cvtColor(input,raw_frame,CV_RGB2GRAY);
			if(first)
			{
				fixed_image = raw_frame.clone();
				detector.detect(raw_frame,keypoint_fixed);
				extractor.compute(raw_frame,keypoint_fixed,fixed_descriptor);
				first = false;
				continue;
			}

			detector.detect(raw_frame,keypoint);
			extractor.compute(raw_frame,keypoint,descriptor);
			//matcher.match(descriptor,fixed_descriptor,matches);
			matcher.radiusMatch(descriptor, fixed_descriptor,matches,100,Mat(),true);
			float sum = 0;
			for(int i=0;i<matches.size();i++)
			{
				data_log << matches[i][1].distance << ", ";
			}
			data_log << endl;

			cout << "Average Distance: " << sum/(float)matches.size() << endl;



			drawMatches(fixed_image,keypoint_fixed,raw_frame,keypoint,matches,output_frame);
			imshow("output",output_frame);
			std::cout << "Num Points: " << keypoint.size() << "Num Matches: " << matches.size() << std::endl;
			//drawKeypoints(raw_frame,keypoint,raw_frame,Scalar(255,0,0));
			//drawKeypoints(raw_frame,keypoint_fixed,raw_frame,Scalar(0,255,0),DrawMatchesFlags::DRAW_OVER_OUTIMG);
			imshow("FLOW!", raw_frame);
		}
		waitKey(1);
	}
	data_log.close();
	return 0;
}
