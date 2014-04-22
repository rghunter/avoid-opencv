#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "FeatureTracker.h"
#include "FlowField.h"
#include "FrameDescriptor.h"

#include <iostream>
#include <signal.h>

using namespace std;

#define MAX_FEATURES 1000

bool isRunning = true;

timespec slot1;
timespec slot2;

timespec *current;
timespec *past;

static void exit_program(int a);
inline void swap(void *a, void *);

int main(void)
{
#if 0
	VideoCapture video(0);
	video.set(CV_CAP_PROP_FRAME_WIDTH,480);
	video.set(CV_CAP_PROP_FRAME_HEIGHT,640);
#else
	cv::VideoCapture video("./video/home_vids/up_driverway_gp_med.mp4");
#endif

	if(!video.isOpened()){
		cout << "ERROR: Could not open file." << endl;
		return 1;
	}

	current = &slot1;
	past = &slot2;

	signal(SIGINT,exit_program);

	cv::Mat output_frame;

	COLA::FlowField field(MAX_FEATURES);

	COLA::FrameDescriptor frame_des_1;
	COLA::FrameDescriptor frame_des_2;

	COLA::FrameDescriptor* train = &frame_des_2;
	COLA::FrameDescriptor* query = &frame_des_1;

	COLA::FeatureTracker flow_tracker(MAX_FEATURES);

	unsigned int frame_count = 0;
	while (isRunning && video.grab())
	{
		query->clear();
		video.retrieve(query->refFrame);
		if(query->refFrame.empty()) {
			cout << "BAD FRAME" << endl;
			continue;
		}

		if(!flow_tracker.generateDescriptors(query->refFrame, *query))
			continue;

		if(train->refFrame.empty())
		{
			swap(query,train);
			continue;
		}

		flow_tracker.frameMatcher(*train, *query, field);

		cv::drawMatches(train->refFrame,train->featurePoints, \
				query->refFrame, query->featurePoints, \
				field.matches, \
				output_frame,cv::Scalar(0,255,0),cv::Scalar(0,0,255));

		cv::imshow("Output",output_frame);


		if(frame_count == 5){ frame_count = 0; swap(query,train); }
		frame_count++;

		cv::waitKey(1);
	}
	return 0;
}

inline void swap(void *a, void *b) {
	void *temp = a;
	a=b;
	b=temp;
}

int runloop(cv::Mat &frame)
{
	return 0;
}

void exit_program(int a) {
	cout << "Closing program due to caught signal" << endl;
	isRunning = false;
}


void fps_delay(int fps)
{

	p

}
