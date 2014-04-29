#include "COLA/FlowField.h"
#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>

using namespace COLA;

cv::Vec2f generate_flow(cv::Point2f start = cv::Point2f(1,1), cv::Point2f end = cv::Point2f(1,2), float time=10){
	cv::Vec2f flow_vector(end-start);
	flow_vector = flow_vector / time;
	return flow_vector;
}

TEST(FlowPoint, Test_Constructor) {
	cv::Point2f start(23,55);
	cv::Point2f end(56,98);
	float time = 0.002;
	cv::Vec2f test_vector = generate_flow(start,end,time);
	FlowPoint test_point(end, test_vector);
	ASSERT_EQ(test_point.magnitude,test_vector);
	ASSERT_EQ(test_point.location, end);
}

TEST(FlowPoint, Test_Math) {
	cv::Point2f point_start(10,5);
	cv::Point2f point_end(11,3);
	float time=10; //ten seconds
	FlowPoint test(point_start, point_end, time);
	ASSERT_EQ(test.magnitude,generate_flow(point_start, point_end, time));
	ASSERT_EQ(test.location,point_end);
}

TEST(FlowPoint, Test_Equal) {
	cv::Point2f point_start(23,66);
	cv::Point2f point_end(66,34);

	float time=3.55;

	FlowPoint test1(point_start,point_end,time);
	FlowPoint test2(point_start,point_end,time);

	ASSERT_EQ(test1,test2);
}

TEST(FlowField, TestConstructor) {
	int size = 10;
	std::vector<FlowPoint> input_vector;
	for(int i=0;i<size;i++){
		input_vector.push_back(FlowPoint(cv::Point2f(i,i),cv::Point2f(i+2,i+2),10));
	}
	FlowField test_field(size);
	for(int i=0;i<size;i++){
		ASSERT_EQ(test_field[i],input_vector[i]);
	}
}

