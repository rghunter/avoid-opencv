/*
 * FlowField.cpp
 *
 *  Created on: Apr 21, 2014
 *      Author: rhunter
 */

#include "FlowField.h"

namespace COLA {
//Flow Point
FlowPoint::FlowPoint(cv::Point2f root, cv::Point2f magnitude) : root(root), magnitude(magnitude) {}

//Flow Field

FlowField::FlowField(int size) : timeDelta_sec(-1.0) { flowField.reserve(size); matches.reserve(size); }

FlowField::~FlowField() { return; }


void FlowField::addFlowPoint(COLA::FlowPoint &flow_point) { flowField.push_back(flow_point); }
void FlowField::addFlowPoint(const COLA::FlowPoint flow_point) { flowField.push_back(flow_point); }

const FlowPoint* FlowField::getFlowPoint(int index) { return &flowField.at(index); }

int FlowField::getSize(void){ return flowField.size(); }
void FlowField::clear(void) { flowField.clear(); matches.clear();}

} /* namespace COLA */
