/*
 * Tau.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: rghunter
 */

#include <COLA/Tau.h>

namespace COLA {

Tau::Tau(float tau, cv::Point2f location) : cv::Point3f(location.x, location.y, tau), tau(tau), location(location) { }
Tau::Tau(COLA::FlowPoint flow_pt, cv::Point2f nodal) { }
Tau::Tau(COLA::FlowPoint flow_pt, cv::Point2f nodal, cv::Point2f global_flow) { }

} /* namespace COLA */
