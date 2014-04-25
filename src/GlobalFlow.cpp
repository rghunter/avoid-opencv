/*
 * GlobalFlow.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "COLA/GlobalFlow.h"

namespace COLA {

cv::Vec2f GlobalFlow::CalculateGlobalFlow(COLA::FlowField &field) {

	cv::Vec2f sum(0,0);
	for (unsigned int i=0; i<field.size(); i++)
		sum += field[i];

	if(field.size() > 0)
		sum *= 1.0/(float)field.size();
	return 	sum;
}



} /* namespace COLA */
