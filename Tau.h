/*
 * Tau.h
 *
 *  Created on: Oct 15, 2012
 *      Author: rghunter
 */

#ifndef TAU_H_
#define TAU_H_

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace OpticalQuad {

class Tau {
public:
	Tau();
	Tau(cv::Point2f tau, cv::KeyPoint location);
	virtual ~Tau();
	cv::Point2f tau;
	cv::KeyPoint location;
};

} /* namespace OpticalQuad */
#endif /* TAU_H_ */
