/*
 * GlobalFlow.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "GlobalFlow.h"

namespace COLA {

cv::Point2f GlobalFlow::CalculateGlobalFlow(COLA::FlowField &field) {

	cv::Point2f sum(0,0);
	for (unsigned int i=0; i<field.flowField.size(); i++) {
		sum += field.flowField[i].magnitude; }

	float size = field.flowField.size();
	float timeD = field.timeDelta_sec;

	float x = (float)sum.x/size/timeD;
	float y = (float)sum.y/size/timeD;

	return 	cv::Point2f(x,y);

}

void GlobalFlow::DrawFlowVector(cv::Mat &output, cv::Mat &input_frame, cv::Point2f &flow_vector)
{
	input_frame.copyTo(output);
	cv::Point2f center_pt(output.cols/2,output.rows/2);
	cv::Point2f mag_vector = center_pt + flow_vector;
	//lets only extract the horizontal portion
	mag_vector.y = center_pt.y;

	cv::line(output,center_pt,mag_vector,cv::Scalar(0,0,255),3);
	return;
}

} /* namespace COLA */
