#include <opencv2/opencv.hpp>

#include "FeatureTracker.h"
#include "FlowField.h"
#include "FrameDescriptor.h"
#include "GlobalFlow.h"

#include <iostream>
#include <signal.h>

using namespace std;

#define MAX_FEATURES 1000

bool isRunning = true;

static void exit_program(int a);
inline void swap(void *a, void *);

int main(void)
{
#if 1
	cv::VideoCapture video(0);
//	video.set(CV_CAP_PROP_FRAME_WIDTH,480);
//	video.set(CV_CAP_PROP_FRAME_HEIGHT,640);
#else
	cv::VideoCapture video("./video/home_vids/up_driverway_gp_med.mp4");
#endif

	if(!video.isOpened()){
		cout << "ERROR: Could not open file." << endl;
		return 1;
	}

	signal(SIGINT,exit_program);

	cv::Mat output_frame;

	COLA::FlowField field(MAX_FEATURES);

	COLA::FrameDescriptor train;
	COLA::FrameDescriptor query;

	COLA::FeatureTracker flow_tracker(MAX_FEATURES);
	COLA::GlobalFlow flow;

	unsigned int frame_count = 0;
	while (isRunning && video.grab())
	{
		query.clear();
		video.retrieve(query.refFrame);
		if(query.refFrame.empty()) {
			cout << "BAD FRAME" << endl;
			continue;
		}

		if(!flow_tracker.generateDescriptors(query.refFrame, query))
			continue;

		if(train.refFrame.empty())
		{
			swap(query,train);
			continue;
		}

		flow_tracker.frameMatcher(train, query, field);
		cv::Point2f flow_vector = flow.CalculateGlobalFlow(field);
		flow.DrawFlowVector(output_frame,query.refFrame,flow_vector);


#if 0
		cv::drawMatches(train.refFrame,train.featurePoints, \
				query.refFrame, query.featurePoints, \
				field.matches, \
				output_frame,cv::Scalar(0,255,0),cv::Scalar(0,0,255));

#endif


		cv::imshow("Output",output_frame);


		if(frame_count == 5){ frame_count = 0; swap(query,train); }
		frame_count++;

		cv::waitKey(1);
	}
	return 0;
}

inline void swap(COLA::FrameDescriptor &a, COLA::FrameDescriptor &b) {
	COLA::FrameDescriptor temp = a;
	a=b;
	b=temp;
}

void exit_program(int a) {
	cout << "Closing program due to caught signal" << endl;
	isRunning = false;
}
