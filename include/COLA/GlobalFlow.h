/*
 * GlobalFlow.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#ifndef GLOBALFLOW_H_
#define GLOBALFLOW_H_

#include <COLA/FlowField.h>

#include <opencv2/opencv.hpp>

namespace COLA {

class GlobalFlow {
public:
	cv::Point2f CalculateGlobalFlow(COLA::FlowField &flowField);
	void DrawFlowVector(cv::Mat &outputFrame, cv::Mat &input_frame, cv::Point2f &flow_vector);
};

} /* namespace COLA */

#endif /* GLOBALFLOW_H_ */
