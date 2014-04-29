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
/**
 * \brief	Calculates the global flow from a COLA::FlowField
 *
 * Sometimes we would like to see the flow of the whole image, we accomplish this by averaging an entire FlowField for a given
 * set of frames.
 */
class GlobalFlow {
private:
	cv::Size frame_size;
public:
	/**
	 * @brief	Constructor (Pre-allocation)
	 */
	GlobalFlow(cv::Size frame_size);
	/**
	 * @brief	Global Flow calculator
	 * @param &flowField COLA::FlowField a reference to the flow field we will average to return the global flow vector.
	 * @returns COLA::FlowPoint the optical flow vector representing the average flow of the image. The location of this vector
	 * 			is assumed to be the camera nodal point.
	 */
	COLA::FlowPoint CalculateGlobalFlow(COLA::FlowField &flowField);
};

} /* namespace COLA */

#endif /* GLOBALFLOW_H_ */
