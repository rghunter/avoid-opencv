#include "FlowField.h"
#include "gtest/gtest.h"

using namespace COLA;

TEST(FlowPoint, Test_Construction_and_Memory) {
	cv::Point2f root(1.0,1.0);
	cv::Point2f mag(2.0,2.0);
	float time = 20.1;

	FlowPoint test(root,mag,time);
	ASSERT_TRUE(root == test.root);
	ASSERT_TRUE(mag == test.magnitude);
	ASSERT_TRUE(time == test.delta_t);

	//Confirm that we store a COPY
	ASSERT_FALSE(&root == &test.root);
	ASSERT_FALSE(&mag == &test.magnitude);
	ASSERT_FALSE(&time == &test.delta_t);
}

TEST(FlowField, TEST_DEFAULT_Construction_and_Memory) {
	FlowField testField(20.0);
	FlowPoint testPoint(cv::Point2f(1,2),cv::Point2f(2,2),10);

	ASSERT_EQ(testField.getLength(), 0);
	testField.addFlowPoint(testPoint);
	ASSERT_EQ(testField.getLength(), 1);

	FlowPoint temp = testField.getFlowPoint(0);
	ASSERT_EQ(testPoint,testField.getFlowPoint(0));


}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
