/*
 * DrawTools.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: rhunter
 */

#include "DrawTools.h"

namespace COLA {

DrawTools::~DrawTools() {
	// TODO Auto-generated destructor stub
}

DrawTools::DrawTools() {
	// TODO Auto-generated constructor stub

}
void DrawTools::DrawGlobalFlowVector(cv::Mat &output, cv::Mat &input_frame, cv::Point2f &flow_vector)
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
