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
	virtual ~DrawTools();
	DrawTools();
	static void DrawGlobalFlowVector(cv::Mat &output, cv::Mat &input_frame, cv::Vec2f &flow_vector);
	static void DrawMatches(cv::Mat &output, COLA::FrameDescriptor &train, COLA::FrameDescriptor &query, COLA::FlowField &field);
};

} /* namespace COLA */

#endif /* DRAWTOOLS_H_ */
