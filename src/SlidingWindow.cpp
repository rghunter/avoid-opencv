/*
 * SlidingWindow.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: rhunter
 */

#include "COLA/SlidingWindow.h"

namespace COLA {

SlidingWindow::SlidingWindow(unsigned int size) :size(size) { }

bool SlidingWindow::getFrameSet(COLA::FrameDescriptor* &train, COLA::FrameDescriptor* &query){
	if(window.size() == size+1){
		window.pop_front();
		train = &window.front();
		query = &window.back();
		return true;
	}
	return false;
}
void SlidingWindow::addFrame(FrameDescriptor &frame){
	window.push_back(frame);
	window.resize(size+1);
}

} /* namespace COLA */
