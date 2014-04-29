/*
 * GlobalFlow.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "COLA/GlobalFlow.h"

namespace COLA {

GlobalFlow::GlobalFlow(cv::Size frame_size) : frame_size(frame_size) {}

COLA::FlowPoint GlobalFlow::CalculateGlobalFlow(COLA::FlowField &field) {

	cv::Vec2f sum(0,0);
	for (unsigned int i=0; i<field.size(); i++)
		sum += field[i].magnitude;

	if(field.size() > 0)
		sum *= 1.0/(float)field.size();
	return 	COLA::FlowPoint(cv::Point2f(frame_size.width/2,frame_size.height/2), sum);
}



} /* namespace COLA */
