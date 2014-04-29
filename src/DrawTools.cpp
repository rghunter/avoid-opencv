/*
 * DrawTools.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: rhunter
 */

#include <COLA/DrawTools.h>

namespace COLA {

void DrawTools::DrawFlowPoint(cv::Mat &output, cv::Mat &input_frame, FlowPoint &flow_vector)
{
	input_frame.copyTo(output);
	cv::Point2f mag_vector = flow_vector.location + cv::Point2f(flow_vector.magnitude);

	cv::line(output,flow_vector.location,mag_vector,cv::Scalar(0,0,255),3);
	return;
}

void DrawTools::DrawMatches(cv::Mat &output, FrameDescriptor &train, FrameDescriptor &query, FlowField &field)
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
void DrawTools::DrawTau(cv::Mat &output, COLA::TauMat &tau_field)
{
	int dialation_size = 5;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2*dialation_size+1, 2*dialation_size+1),cv::Point(dialation_size,dialation_size));
	cv::dilate(tau_field,output,element);
}

} /* namespace COLA */
