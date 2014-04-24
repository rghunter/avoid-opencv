#include <opencv2/opencv.hpp>

#include <COLA/COLA.h>

#include <iostream>
#include <signal.h>

using namespace std;

#define MAX_FEATURES 1000

bool isRunning = true;

static void exit_program(int a);
inline void swap_pointer(COLA::FrameDescriptor* &a, COLA::FrameDescriptor* &b);

int main(void)
{
#if 0
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

	cv::Mat match_frame, flow_frame;

	COLA::FlowField field(MAX_FEATURES);

	//lets pull the first frame off so we can get its size

	cv::Mat size_frame;
	video >> size_frame;

	int height = size_frame.rows;
	int width = size_frame.cols;

	cv::Rect roi(0,height/3,width,height/3);

	COLA::FrameDescriptor *train = new COLA::FrameDescriptor(MAX_FEATURES,roi);
	COLA::FrameDescriptor *query = new COLA::FrameDescriptor(MAX_FEATURES,roi);

	COLA::FeatureTracker flow_tracker(MAX_FEATURES);
	COLA::GlobalFlow flow;

	COLA::Time &fpsControl = *COLA::Time::Instance(10,false);

	bool lag = false;

	unsigned int frame_count = 0;
	while (isRunning && video.grab())
	{
		query->clear();
		video.retrieve(query->refFrame);
		if(query->refFrame.empty()) {
			cout << "BAD FRAME" << endl;
			continue;
		}

		if(!flow_tracker.generateDescriptors(*query))
			continue;

		if(train->refFrame.empty())
		{
			swap_pointer(query,train);
			continue;
		}

		flow_tracker.frameMatcher(*train, *query, field);
		cv::Point2f flow_vector = flow.CalculateGlobalFlow(field);

		//draw the HORIZONTAL portion of the vector
		COLA::DrawTools::DrawGlobalFlowVector(flow_frame,query->refFrame,flow_vector);

		//draw the matches
		COLA::DrawTools::DrawMatches(match_frame,*train,*query,field);

		//Push to display
		if (lag) {
			cv::putText(flow_frame,"LAG DETECTED", cv::Point2f(10,30), cv::FONT_HERSHEY_SIMPLEX,1,cv::Scalar(0,0,255),2);
		}

		cv::imshow("Optical Flow",flow_frame);
		cv::imshow("Matches",match_frame);

		if(frame_count == 5){ frame_count = 0; swap_pointer(query,train); }
		frame_count++;

		cv::waitKey(1);

		lag = fpsControl.delay();

	}
	return 0;
}

inline void swap_pointer(COLA::FrameDescriptor* &a, COLA::FrameDescriptor* &b) {
	COLA::FrameDescriptor* temp = a;
	a=b;
	b=temp;
}

void exit_program(int a) {
	cout << "Closing program due to caught signal" << endl;
	isRunning = false;
}
