/*
 * Tau.cpp
 *
 *  Created on: Oct 15, 2012
 *      Author: rghunter
 */

#include <COLA/Tau.h>
#include <iostream>
namespace COLA {

Tau::Tau(float tau, cv::Point2f location) : cv::Point3f(location.x, location.y, tau), tau(tau), location(location) { }
Tau::Tau(COLA::FlowPoint &flow_pt, cv::Point2f nodal) : location(flow_pt.location), nodal(nodal) {
	cv::Vec2f AN(nodal-flow_pt.location);
	tau = cv::norm(AN) / cv::norm((flow_pt.dot(AN) / AN.dot(AN))*AN);
	cv::Point3f(location.x,location.y,tau);
}

TauMat::TauMat(cv::Size mat_size, vector<COLA::Tau> tau_field) {
	cv::Mat(mat_size,CV_8UC1,uchar(1));
	create(mat_size,CV_8UC1);
	float sum=0;
	for(unsigned int i=0;i<tau_field.size();i++){
			sum += tau_field[i].tau;
			at<uchar>(tau_field[i].location) = (uchar)(tau_field[i].tau);
		}
	std::cout << "Average Tau: " << sum/tau_field.size() << std::endl;
}
TauMat::TauMat(cv::Size mat_size, COLA::FlowField &field)
{

}



} /* namespace COLA */
