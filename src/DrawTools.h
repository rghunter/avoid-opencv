/*
 * DrawTools.h
 *
 *  Created on: Apr 23, 2014
 *      Author: rhunter
 */

#ifndef DRAWTOOLS_H_
#define DRAWTOOLS_H_

#include <opencv2/opencv.hpp>

namespace COLA {

class DrawTools {
public:
	virtual ~DrawTools();
	DrawTools();
	static void DrawGlobalFlowVector(cv::Mat &output, cv::Mat &input_frame, cv::Point2f &flow_vector);
};

} /* namespace COLA */

#endif /* DRAWTOOLS_H_ */
