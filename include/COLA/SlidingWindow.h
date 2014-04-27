/*
 * SlidingWindow.h
 *
 *  Created on: Apr 27, 2014
 *      Author: rhunter
 */

#ifndef SLIDINGWINDOW_H_
#define SLIDINGWINDOW_H_

namespace COLA {

class SlidingWindow {

private:
	int size;
	std::deque<COLA::FrameDescriptor> window;
public:
	SlidingWindow(int size);
	bool getSet(COLA::FrameDescriptor* &train, COLA::FrameDescriptor* &query);
	void addFrame(COLA::FrameDescriptor &frame);
};

} /* namespace COLA */

#endif /* SLIDINGWINDOW_H_ */
