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

	cv::line(output,center_pt,mag_vector,cv::Scalar(0,0,255),3);
	return;
}

void DrawTools::DrawMatches(cv::Mat &output, COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field)
{
	cv::Mat train_frame(train.refFrame);
	cv::Mat query_frame(query.refFrame);

	std::vector<char> maskRoot;
	maskRoot.push_back('1');

	std::vector<std::vector<char> > mask;
	for(unsigned int i=0;i<field.matches.size();i++)
		mask.push_back(maskRoot);

	//Draw the ROI rectangles
	cv::rectangle(train_frame, train.roi_rect, cv::Scalar(255,255,255));
	cv::rectangle(query_frame, query.roi_rect, cv::Scalar(255,255,255));

	cv::drawMatches(train_frame,train.featurePoints, \
				query_frame, query.featurePoints, \
				field.matches, \
				output,cv::Scalar(0,255,0),cv::Scalar(0,0,255), mask,2);
}




} /* namespace COLA */
