#include <opencv2/opencv.hpp>

#include <COLA/COLA.h>

#include <iostream>
#include <signal.h>

using namespace std;

#define MAX_FEATURES 3000

bool isRunning = true;

static void exit_program(int a);
inline void swap_pointer(COLA::FrameDescriptor* &a, COLA::FrameDescriptor* &b);

int main(void)
{
	cv::VideoCapture video("./video/home_vids/up_driverway_gp_med.mp4");

	if(!video.isOpened()){
		cout << "ERROR: Could not open file." << endl;
		return 1;
	}

	signal(SIGINT,exit_program);

	cv::Mat match_frame, flow_frame, tau_frame;

	COLA::FlowField field(MAX_FEATURES);

	//lets pull the first frame off so we can get its size

	cv::Mat size_frame;
	video >> size_frame;

	int height = size_frame.rows;
	int width = size_frame.cols;

	cv::Rect roi(0,height/3,width,height/3);

	COLA::FrameDescriptor *train = new COLA::FrameDescriptor(MAX_FEATURES,roi);
	COLA::FrameDescriptor *query = new COLA::FrameDescriptor(MAX_FEATURES,roi);

	COLA::DrawTools draw_cola;

	COLA::FeatureTracker flow_tracker(MAX_FEATURES);
	COLA::GlobalFlow flow(size_frame.size());

	//Set the singleton time control
	COLA::Time &fpsControl = *COLA::Time::Instance(10,false);

	bool lag = false;
	int frames=0;

	while (isRunning && video.grab())
	{
		query->reset();
		video.retrieve(query->refFrame);
		if(query->refFrame.empty()) {
			cout << "BAD FRAME" << endl;
			continue;
		}

		if(!flow_tracker.generateDescriptors(*query)) //If we did not get any descriptors, go to start of loop.
			continue;

		if(train->refFrame.empty()) //on the first run, we need to populate train with the first good frame, reset loop
		{
			swap_pointer(query,train);
			continue;
		}

		flow_tracker.frameMatcher(*train, *query, field);
		COLA::FlowPoint flow_vector = flow.CalculateGlobalFlow(field);

/********/
		vector<COLA::Tau> tau_field;
		//Calcualte Tau
		for(unsigned int i=0;i<field.size();i++)
			tau_field.push_back(COLA::Tau(field[i],cv::Point2f(size_frame.cols/2,size_frame.rows/2)));
		cout << "field size: " << tau_field.size() << endl;
		COLA::TauMat tauMatrix(size_frame.size(),tau_field);
/********/

		//draw the matches
		draw_cola.DrawMatches(match_frame,*train,*query,field);
		draw_cola.DrawFlowPoint(flow_frame,query->refFrame,flow_vector);
		draw_cola.DrawTau(tau_frame, tauMatrix);

		if (lag) {
			cv::putText(flow_frame,"LAG DETECTED", cv::Point2f(10,30), cv::FONT_HERSHEY_SIMPLEX,1,cv::Scalar(0,0,255),2);
		}

		cv::imshow("Optical Flow",flow_frame);
		cv::imshow("Matches",match_frame);
		if(tau_frame.rows > 0 && tau_frame.cols > 0)
			cv::imshow("TAU",tau_frame);

		cv::waitKey(1);

		lag = fpsControl.delay();

//update the train frame at a regular interval
		if(frames == 3){
			swap_pointer(query,train);
			frames=0;
		}else{ frames++; }
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
