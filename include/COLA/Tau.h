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

/**
 * \brief	Calculate the Tau value for a FlowPoint
 *
 * Tau is a mathematical representation of the time to collision with an object. Tau is effectively the orthogonal projection of the
 * flow vector onto the nodal point.
 * \f[\tau = \frac{||\vec{AN}||} {||\vec{AN}_{flow}||}\f]
 */
class Tau : public cv::Point3f {
public:
	float tau; /**< The time until collision in seconds */
	cv::Point2f location; /**< The current location of the point on the image plane */
	cv::Point2f nodal; /**< The nodal point of the camera (in reference to the image plane) used to calculate the Tau value */

	/**
	 * @brief	Constructor (Pre-calculated Tau)
	 *
	 * @param tau float the time until impact
	 * @param location cv::Point2f the location of the tau point.
	 */
	Tau(float tau, cv::Point2f location);

	/**
	 * @brief	Constructor (Calculate Tau from FlowPoint)
	 *
	 * This constructor calculates tau from a flow point and specified nodal (in relation to the FlowPoint.
	 * @param &flow_pt COLA::FlowPoint the flow point we will calculate tau for
	 * @param &nodal cv::Point2f the nodal point of the camera (usually the center point of the frame)
	 */
	Tau(COLA::FlowPoint &flow_pt, cv::Point2f &nodal);

};

/**
 * \brief	Tau Mat UNSTABLE
 *
 * This object is currently not in a stable state, however, it is designed to report the tau values in a matrix normalized to uchar for
 * display
 */
class TauMat : public cv::Mat {
public:

	/**
	 * @brief	Constructor (Create from vector<COLA::Tau>)
	 *
	 * This constructor creates a TauMat from a tau_field vector of COLA::Tau points
	 * @param mat_size cv::Size the size of the output matrix (usually same as the frame size)
	 * @param tau_field vector<COLA::Tau> a vector of COLA::Tau points
	 */
	TauMat(cv::Size mat_size, vector<COLA::Tau> tau_field);

};

}

#endif /* TAU_H_ */
