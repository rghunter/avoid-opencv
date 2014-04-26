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
	cv::Point2f nodal;
	Tau(float tau, cv::Point2f location);
	Tau(COLA::FlowPoint &flow_pt, cv::Point2f nodal);
};

class TauMat : public cv::Mat {
public:
	TauMat(cv::Size mat_size, vector<COLA::Tau> tau_field);
	TauMat(cv::Size mat_size, COLA::FlowField &field);

};

}

#endif /* TAU_H_ */
