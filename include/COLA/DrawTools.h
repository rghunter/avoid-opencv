/*
 * DrawTools.h
 *
 *  Created on: Apr 23, 2014
 *      Author: rhunter
 */

#ifndef DRAWTOOLS_H_
#define DRAWTOOLS_H_

#include <opencv2/opencv.hpp>
#include <COLA/FlowField.h>
#include <COLA/FrameDescriptor.h>
#include <COLA/Tau.h>

namespace COLA {

class DrawTools {
public:
	void DrawFlowPoint(cv::Mat &output, cv::Mat &input_frame, FlowPoint &flow_vector);
	void DrawMatches(cv::Mat &output, FrameDescriptor &train, FrameDescriptor &query, FlowField &field);
	void DrawTau(cv::Mat &output, COLA::TauMat &tau_field);
};

} /* namespace COLA */

#endif /* DRAWTOOLS_H_ */
