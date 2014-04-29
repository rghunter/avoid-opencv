#include <opencv2/opencv.hpp>

#include <COLA/COLA.h>

#include <iostream>
#include <signal.h>

using namespace std;

#define MAX_FEATURES 3000

bool isRunning = true;

static void exit_program(int a);
inline void swap_pointer(COLA::FrameDescriptor* &a, COLA::FrameDescriptor* &b);

void rotate(cv::Mat& src, double angle, cv::Mat& dst)
{
    int len = std::max(src.cols, src.rows);
    cv::Point2f pt(len/2., len/2.);
    cv::Mat r = cv::getRotationMatrix2D(pt, angle, 1.0);

    cv::warpAffine(src, dst, r, cv::Size(len, len));
}

int main(void)
{

	//cv::VideoCapture video("./video/home_vids/up_driverway_gp_med.mp4");
	cv::VideoCapture video("./video/crashEmily.MOV");

	video.set(cv::CAP_PROP_FRAME_WIDTH, 160);
	video.set(cv::CAP_PROP_FRAME_HEIGHT, 120);

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
	cv::Mat rotated_size_frame;
	rotate(size_frame,-90.0,rotated_size_frame);

	int height = 640;
	float width_mul_factor = (float)height/size_frame.cols;

	int width = size_frame.rows*width_mul_factor;

	cv::Size frame_size(height,width);

	cv::Rect roi(0,height/3,width,height/3);

	COLA::FrameDescriptor *train = new COLA::FrameDescriptor(MAX_FEATURES);
	COLA::FrameDescriptor *query = new COLA::FrameDescriptor(MAX_FEATURES);

	COLA::DrawTools draw_cola;

	COLA::FeatureTracker flow_tracker(MAX_FEATURES);
	COLA::GlobalFlow flow(size_frame.size());

	//Set the singleton time control
	COLA::Time &fpsControl = *COLA::Time::Instance(10,false);

	bool lag = false;
	int frames=1;

	while (isRunning && video.grab())
	{
		cv::Mat rawFrame;
		lag = fpsControl.delay();
		query->reset();
		video.retrieve(rawFrame);
		if(rawFrame.empty()) {
			cout << "BAD FRAME" << endl;
			continue;
		}

		cv::Mat rotated_frame;
		rotate(rawFrame,-90.0,rotated_frame);
		cv::resize(rotated_frame,query->refFrame,frame_size);


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
		cv::Point2f nodal(size_frame.cols/2,size_frame.rows/2);
		for(unsigned int i=0;i<field.size();i++)
			if(cv::norm(field[i].magnitude) > 0)
				tau_field.push_back(COLA::Tau(field[i],nodal));

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


//update the train frame at a regular interval
		if(frames == 3){
			swap_pointer(query,train);
			frames=1;
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
