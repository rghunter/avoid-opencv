/*
 * FlowField.cpp
 *
 *  Created on: Apr 21, 2014
 *      Author: rhunter
 */

#include "COLA/FlowField.h"
#include <stdexcept>

namespace COLA {
//Flow Point
FlowPoint::FlowPoint(cv::Point2f location, cv::Vec2f magnitude) : location(location), magnitude(magnitude) {}

FlowPoint::FlowPoint(cv::Point2f start, cv::Point2f end, float time_delta) : location(end) {
	if(time_delta == 0)
		throw std::logic_error("Time is defined as ZERO!!! WTF!!!!");
	magnitude = end - start;
	magnitude = magnitude/time_delta;
}
bool FlowPoint::operator== (const FlowPoint &other) const{
	return (other.magnitude == magnitude && other.location == location);
}


//Flow Field

FlowField::FlowField(int size) {matches.reserve(size); }
void FlowField::reset(void) { clear(); matches.clear();}

} /* namespace COLA */
