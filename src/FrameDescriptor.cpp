/*
 * FrameDescriptor.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "COLA/FrameDescriptor.h"

namespace COLA {

FrameDescriptor::FrameDescriptor(int numberOfFeatures, cv::Rect roi_rect) : numberOfFeatures(numberOfFeatures), frame_number(0), roi_rect(roi_rect) {

	//Allocate KeyPoint Vector
	featurePoints.reserve(numberOfFeatures);

	//if we have an roi to generate, set the flag, otherwise, set process frame to point to refFrame
	has_been_normalized = false;
	if(roi_rect.area() > 0){
		roi_set = true;
		roi_offset = cv::Point2f(roi_rect.x,roi_rect.y);
		process_frame = new cv::Mat();
	}else{
		roi_set = false;
		roi_offset = cv::Point2f(0,0);
		process_frame = &refFrame;
	}

}

void FrameDescriptor::normalizeKeypoints(void) {
	if(roi_set==true && has_been_normalized == false)
		for(unsigned int i=0;i<featurePoints.size();i++)
			featurePoints[i].pt += roi_offset;
	has_been_normalized = true;
}
int FrameDescriptor::MaxFeatures(void) { return numberOfFeatures; }

void FrameDescriptor::clear(void) {
	featurePoints.clear();
	has_been_normalized = false;
	//timestamp = NULL;
}
} /* namespace COLA */
