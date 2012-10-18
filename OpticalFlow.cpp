#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "ObstacleDetector.h"
#include <sys/time.h>
#include <unistd.h>
#include <signal.h>

#define ACC_EUCLID 60
#define RECORD_VIDEO 1

#define NUM_BINS	11

using namespace cv;
using namespace std;

bool running;

Mat raw_frame, grayscale_frame, initial_image, output_frame;
vector<KeyPoint> keypoints_train, keypoints_query;
vector<DMatch> matches;

float euclid_distance(KeyPoint pt1, KeyPoint pt2)
{
	return (float)sqrt(pow((double)(pt1.pt.x - pt2.pt.x),2)+pow((double)(pt1.pt.y - pt2.pt.y),2));
}
void end_signal(int a)
{
	cout << "Closing program due to caught signal" << endl;
	running = false;
}

int main(void)
{


	VideoCapture video("./video/home_vids/round_house_gp_med.mp4");
	//VideoCapture video(0);



//	video.set(CV_CAP_PROP_FRAME_WIDTH,320);
//	video.set(CV_CAP_PROP_FRAME_HEIGHT,240);


	if(!video.isOpened()){
		cout << "ERROR: Could not open file." << endl;
		return 1;
	}

	int height = video.get(CV_CAP_PROP_FRAME_HEIGHT);
	int width = video.get(CV_CAP_PROP_FRAME_WIDTH);
	double vid_fps = video.get(CV_CAP_PROP_FPS);

	cout << "FPS: " << vid_fps << endl;

	ObstacleDetector detector(1500,4,100,"./calibration_files/gopro.xml",height,width);

	vector<vector<float> > bins;
	bins.resize(NUM_BINS);
	//for(int i=0;i<bins.size();i++)
	//	bins[i].resize(40);
	float bin_loc[NUM_BINS];

	float iterator = width / NUM_BINS;

	for(int i=0;i<NUM_BINS;i++)
		bin_loc[i] = iterator * i + iterator / 2;


	ofstream data_log0, data_log1;
	data_log0.setf(ios::floatfield);
	data_log0.precision(2);
	data_log0.open("./data/hitMissEval.csv");
	data_log0 << "Hit, Miss, Total" << endl;

	struct timespec start, end;
	float fps;

	running = true;

	int frame_count = 0;

	cout << "Height: " << height << " Width: " << width << endl;


#if RECORD_VIDEO
	VideoWriter recorder("./output.avi", CV_FOURCC('D','I','V','X'),15,Size(width*2,height*2),true);
	if(!recorder.isOpened())
	{
		cout << "Could not open file for recording" << endl;
		return 0;
	}
	cout << "Begin.... Please Wait..." << endl;
#endif

	signal(SIGINT,end_signal);

	while(running && video.grab())
	{
		clock_gettime(CLOCK_MONOTONIC,&start);
		video.retrieve(raw_frame);
		frame_count++;
		if(!(frame_count%1 == 0))
		{
			continue;
		}
		if(raw_frame.empty())
		{
			cout << "BAD FRAME" << endl;
			continue;
		}
		detector.matchFrame(raw_frame);
		clock_gettime(CLOCK_MONOTONIC,&end);
#if 0
		vector<char> MASK_G,MASK_B;
		float correct, incorrect;
		correct = 0;
		incorrect = 0;
		for(uint i=0;i<detector.matches.size();i++)
		{

			KeyPoint pt1 = detector.keypoint_train[detector.matches[i].queryIdx];
			KeyPoint pt2 = detector.keypoint_query[detector.matches[i].trainIdx];

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
#endif
		if(!detector.initial_frame.empty() && !detector.undistorted_frame.empty())
		{
			rectangle(detector.initial_frame,*detector.roi_rec,Scalar(255,0,0));
			rectangle(detector.undistorted_frame,*detector.roi_rec,Scalar(255,0,0));

			drawMatches(detector.initial_frame,detector.keypoint_train,detector.undistorted_frame,detector.keypoint_query,detector.matches,output_frame,Scalar(0,255,0),Scalar(0,0,255));
			fps = 1.0/((end.tv_sec + end.tv_nsec/1000000000.0) - (start.tv_sec + start.tv_nsec/1000000000.0));
			char output_text[30];
			sprintf(output_text,"FPS: %4.2f",fps);
			putText(output_frame,output_text,Point(10,30),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),2);

			Mat tauPlot, bin_tauPlot;

			raw_frame.copyTo(tauPlot);
			line(tauPlot,Point2f(0,tauPlot.rows/2.0f),
					Point2f(tauPlot.cols,tauPlot.rows/2.0f),Scalar(0,255,0));

			tauPlot.copyTo(bin_tauPlot);

			float scale_space = tauPlot.rows/50.0f;
			float center = tauPlot.rows/2.0f;
			float it = bin_tauPlot.cols / NUM_BINS;


			for(unsigned int i=0;i<detector.tauVals.size();i++)
				bins[floor(detector.tauVals[i].location.pt.x/it)].push_back(
						detector.tauVals[i].tau);
#if 1
			for(int i=0;i<NUM_BINS;i++)
			{
				float avg = 0;
				if(bins[i].size() == 0)
					continue;
				for(int t=0;t<bins[i].size();t++)
					avg += bins[i][t];
				avg = avg / bins[i].size();
				float std_av = 0;
				for(int t=0;t<bins[i].size();t++)
					std_av += pow((bins[i][t]-avg),2);
				float std = sqrt(std_av/bins[i].size()-1);
				float fil_av=0;
				int num = 0;
				for(int t=0;t<bins[i].size();t++)
					if((bins[i][t] > (avg-std)) && (bins[i][t] < (avg+std)))
					{
						num++;
						fil_av += bins[i][t];
					}
				fil_av = fil_av / num;
				bins[i].clear();
				float height_fil = center - (fil_av * scale_space);
				float height_ufil = center - (avg * scale_space);
				Point2f origin_fil(bin_loc[i]-2,(bin_tauPlot.rows/2.0f));
				Point2f final_fil(bin_loc[i]-2,height_fil);
				line(bin_tauPlot,origin_fil,final_fil,Scalar(0,0,255),3);

				Point2f origin_ufil(bin_loc[i]+2,(bin_tauPlot.rows/2.0f));
				Point2f final_ufil(bin_loc[i]+2,height_ufil);
			//	line(bin_tauPlot,origin_ufil,final_ufil,Scalar(0,255,0),3);
			}
#endif
			for(unsigned int i=0;i<detector.tauVals.size();i++)
			{
				Point2f location = detector.tauVals[i].location.pt;
				float tau = detector.tauVals[i].tau;
				float height = scale_space * tau;
				float final_height = center - height;
				Point2f origin(location.x,tauPlot.rows/2.0f);
				line(tauPlot,origin,Point2f(location.x,final_height),Scalar(0,0,255),2);
			}

			Size size(output_frame.cols,output_frame.rows+tauPlot.rows);

			Mat outImg(size,CV_MAKETYPE(output_frame.depth(),3));
			Mat top,bottom_left,bottom_right;

			putText(tauPlot,"Tau X Plot",Point(10,30),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),2);
			putText(bin_tauPlot,"Binned Tau X Plot",Point(10,30),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),2);


			top = outImg(Rect(0,0,output_frame.cols,output_frame.rows));
			bottom_right = outImg(Rect(tauPlot.cols,output_frame.rows,bin_tauPlot.cols,bin_tauPlot.rows));
			bottom_left = outImg(Rect(0,output_frame.rows,tauPlot.cols,tauPlot.rows));
			output_frame.copyTo(top);
			tauPlot.copyTo(bottom_left);
			bin_tauPlot.copyTo(bottom_right);

			imshow("combined",outImg);



			//imshow("output",output_frame);
			//imshow("Tau Plot",tauPlot);
			//imshow("Binned Tau Plot",bin_tauPlot);

#if RECORD_VIDEO
			recorder << outImg;
#endif
			cout << "\033[A\033[2K\033[A\033[2K\033[A\033[2K\033[A\033[2K\033[A\033[2K\033[A\033[2K" << flush;
			cout << "\n\nFPS: " << fps << "\nNum Track: " << detector.keypoint_train.size() << endl;
			cout << "Height: " << output_frame.rows << " Width: " << output_frame.cols << endl;
			cout << "Channels: " << output_frame.channels() << endl;
		}

		waitKey(1);
	}
	data_log0.close();
	video.release();
#if RECORD_VIDEO
	recorder.release();
#endif
	cout << "Done!" << endl;
	return 0;
}
