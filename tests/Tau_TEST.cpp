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
	cv::Point2f nodal(7,7);
	cv::Point2f origin(0,0);
	cv::Point2f final(5,6);
	float time = 1.0;

	FlowPoint BAflow(origin,final,time);
	cv::Vec2f AN(nodal-final);

	Tau test_tau(BAflow,nodal);

	float tau = cv::norm(AN)/cv::norm(AN*(BAflow.magnitude.dot(AN)/AN.dot(AN)));

	ASSERT_EQ(tau,test_tau.tau);

}


/*//This test currently fails because the old method of calculating Tau isnt compatible with the orthoganal projection
TEST(Tau, Test_Math) {
	cv::Point2f nodal(7,7);
	cv::Point2f origin(0,0);
	cv::Point2f final(5,6);
	float time = 1.0;

	FlowPoint test_point(origin,final,time);

	FlowPoint actual(origin,test_point.magnitude);
	Tau test_tau(actual,nodal);

	float dist1 = cv::norm(nodal-origin);
	float dist2 = cv::norm(final-nodal);

	float tau = dist2/abs((dist2-dist1)/time);

	ASSERT_EQ(tau,test_tau.tau);

}*/
