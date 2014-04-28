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
private:
	cv::Size frame_size;
public:
	GlobalFlow(cv::Size frame_size);
	COLA::FlowPoint CalculateGlobalFlow(COLA::FlowField &flowField);
};

} /* namespace COLA */

#endif /* GLOBALFLOW_H_ */
