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

namespace COLA {

class DrawTools {
public:
	void DrawFlowPoint(cv::Mat &output, cv::Mat &input_frame, COLA::FlowPoint &flow_vector);
	void DrawMatches(cv::Mat &output, COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field);
};

} /* namespace COLA */

#endif /* DRAWTOOLS_H_ */
