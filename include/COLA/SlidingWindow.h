/*
 * SlidingWindow.h
 *
 *  Created on: Apr 27, 2014
 *      Author: rhunter
 */

#ifndef SLIDINGWINDOW_H_
#define SLIDINGWINDOW_H_

#include "COLA/FrameDescriptor.h"
#include <deque>

namespace COLA {

class SlidingWindow {

private:
	unsigned int size;
	std::deque<COLA::FrameDescriptor> window;
public:
	SlidingWindow(unsigned int size);
	bool getFrameSet(COLA::FrameDescriptor* &train, COLA::FrameDescriptor* &query);
	void addFrame(COLA::FrameDescriptor &frame);
};

} /* namespace COLA */

#endif /* SLIDINGWINDOW_H_ */
