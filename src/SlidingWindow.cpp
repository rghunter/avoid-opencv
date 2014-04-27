/*
 * SlidingWindow.cpp
 *
 *  Created on: Apr 27, 2014
 *      Author: rhunter
 */

#include "COLA/SlidingWindow.h"
#include "COLA/FrameDescriptor.h"

namespace COLA {

SlidingWindow::SlidingWindow(int size) :size(size) { }

SlidingWindow::getFrameSet(COLA::FrameDescriptor* &train, COLA::FrameDescriptor* &query){
	if(window.size() == size+1){
		window.pop_front();
		train = &window.front();
		query = &window.end();
		return true;
	}
	return false;
}
SlidingWindow::addFrame(FrameDescriptor &frame){
	window.push_back(frame);
	window.resize(size+1);
}

} /* namespace COLA */
