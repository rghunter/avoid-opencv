/*
 * TimeDelay.cpp
 *	This attempts to ensure realtime operation and defines how the program sees time.
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "COLA/Time.h"

#define SPUSec 1000000.0
#define NPUSec 0.001

namespace COLA {

Time* Time::_instance = 0;

Time* Time::Instance(int fps, bool live) {
	if (_instance==0) {
		_instance = new Time(fps,live);
	}
	return _instance;
}

Time::Time(int fps, bool live) : live(live) {
	time_delayS = (1.0/(float)fps);
	time_delayNS =	time_delayS*SPUSec;
	isLag = false;
	frames = 0;
	clock_gettime(CLOCK_MONOTONIC, &past);
}

bool Time::delay() {
	clock_gettime(CLOCK_MONOTONIC, &current);
	int elapsed_time = (int)((current.tv_sec*SPUSec + current.tv_nsec*NPUSec) - (past.tv_sec*SPUSec + past.tv_nsec*NPUSec));
	int remaining_time = time_delayNS - elapsed_time;
	if (remaining_time > 0){
		usleep(remaining_time);
		isLag=false;
	}else{
		isLag=true;
	}
	frames++;
	std::swap(current,past);
	return isLag;
}

float Time::timeElapsed(COLA::FrameDescriptor &start, COLA::FrameDescriptor &end) {
	if(live)
		return (end.timestamp.tv_sec + end.timestamp.tv_nsec/1000000000.0) - (start.timestamp.tv_sec + start.timestamp.tv_nsec/1000000000.0);
	return (end.frame_number - start.frame_number)*time_delayS;
}

void Time::setTime(COLA::FrameDescriptor &frame)
{
	frame.frame_number = frames;
	if(live)
		clock_gettime(CLOCK_MONOTONIC,&frame.timestamp);
}


} /* namespace COLA */
