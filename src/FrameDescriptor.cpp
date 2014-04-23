/*
 * FrameDescriptor.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "FrameDescriptor.h"

namespace COLA {

FrameDescriptor::FrameDescriptor(int numberOfFeatures, cv::Rect roi_rect) : numberOfFeatures(numberOfFeatures), roi_rect(roi_rect) {

	//Allocate KeyPoint Vector
	featurePoints.reserve(numberOfFeatures);

	//Allocate Matrix not entirely nessesary as FREAK will do this on the first run
}

FrameDescriptor::~FrameDescriptor() { }

int FrameDescriptor::MaxFeatures(void) { return numberOfFeatures; }

void FrameDescriptor::clear(void) {
	featurePoints.clear();
	//timestamp = NULL;
}
} /* namespace COLA */
