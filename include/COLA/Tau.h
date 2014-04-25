/*
 * Tau.h
 *
 *  Created on: Oct 15, 2012
 *      Author: rghunter
 */

#ifndef TAU_H_
#define TAU_H_

#include <opencv2/opencv.hpp>
#include <COLA/FlowField.h>

namespace COLA {

class Tau : public cv::Point3f {
public:
	float tau;
	cv::Point2f location;
	Tau(float tau, cv::Point2f location);
	Tau(COLA::FlowPoint flow_pt, cv::Point2f nodal);
	Tau(COLA::FlowPoint flow_pt, cv::Point2f nodal, COLA::FlowPoint global_flow);
};

}

#endif /* TAU_H_ */
