/*
 * FlowField.cpp
 *
 *  Created on: Apr 21, 2014
 *      Author: rhunter
 */

#include "COLA/FlowField.h"

namespace COLA {
//Flow Point
FlowPoint::FlowPoint(cv::Point2f location, cv::Vec2f magnitude) : cv::Vec2f(magnitude), location(location) {}
FlowPoint::FlowPoint(cv::Point2f start, cv::Point2f end, float time_delta) : location(end) {
	cv::Vec2f magnitude = end-start;
	magnitude *= 1.0/time_delta;
	cv::Vec2f(diff_vector);
}

//Flow Field

FlowField::FlowField(int size) {matches.reserve(size); }
FlowField::~FlowField() { return; }
void FlowField::reset(void) { clear(); matches.clear();}

} /* namespace COLA */
