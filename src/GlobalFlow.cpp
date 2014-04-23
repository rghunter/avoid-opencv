/*
 * GlobalFlow.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "GlobalFlow.h"

namespace COLA {

cv::Point2f GlobalFlow::CalculateGlobalFlow(COLA::FlowField &field) {

	cv::Point2f sum(0,0);
	for (unsigned int i=0; i<field.flowField.size(); i++) {
		sum += field.flowField[i].magnitude; }

	float size = field.flowField.size();
	float timeD = field.timeDelta_sec;

	float x = (float)sum.x/size/timeD;
	float y = (float)sum.y/size/timeD;

	return 	cv::Point2f(x,y);

}



} /* namespace COLA */
