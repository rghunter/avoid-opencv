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
	cv::Vec2f CalculateGlobalFlow(COLA::FlowField &flowField);
};

} /* namespace COLA */

#endif /* GLOBALFLOW_H_ */
