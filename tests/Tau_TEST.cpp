#include "COLA/Tau.h"
#include "COLA/FlowField.h"
#include "gtest/gtest.h"
#include <opencv2/opencv.hpp>

using namespace COLA;

TEST(Tau, Test_Constructor) {
	float tau=0.0002;
	cv::Point2f location(1,2);
	cv::Point3f threeDRep(location.x,location.y,tau);
	Tau test_tau(tau,location);

	ASSERT_EQ(threeDRep,test_tau);
	ASSERT_EQ(location,test_tau.location);
	ASSERT_EQ(tau,test_tau.tau);
}

TEST(Tau, Test_Math) {
	cv::Point2f nodal(10,10);
	cv::Point2f origin(0,0);
	cv::Point2f final(5,5);

	float time = 1.0;

	FlowPoint test_point(origin,final,time);
	Tau test_tau(test_point,nodal);

	ASSERT_EQ(cv::norm(test_point.magnitude),test_tau.tau);

}
