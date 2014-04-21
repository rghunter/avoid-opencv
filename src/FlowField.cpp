/*
 * FlowField.cpp
 *
 *  Created on: Apr 21, 2014
 *      Author: rhunter
 */

#include "FlowField.h"

namespace COLA {
//Flow Point
FlowPoint::FlowPoint(cv::KeyPoint root, cv::KeyPoint magnitude, float delta_t) : root(root), magnitude(magnitude), delta_t(delta_t) {}

//Flow Field

FlowField::FlowField(float delta_ms) : timeDelta(delta_ms) { return; }

FlowField::FlowField(float delta_ms, int size) : timeDelta(delta_ms){ flowField.reserve(size); }

FlowField::~FlowField() { return; }

void FlowField::addFlowPoint(const COLA::FlowPoint *flow_point) { flowField.push_back(*flow_point); }

const FlowPoint* FlowField::getFlowPoint(int index) { return &flowField.at(index); }

int FlowField::getLength(void){ return flowField.size(); }

float FlowField::getTimeDelta(void) { return timeDelta; }

} /* namespace COLA */
