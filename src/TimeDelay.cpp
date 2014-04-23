/*
 * TimeDelay.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "TimeDelay.h"

#define SPUSec 1000000.0
#define NPUSec 0.001

namespace COLA {

TimeDelay::TimeDelay(int fps) {
	time_delay = (1.0/(float)fps)*SPUSec;
	clock_gettime(CLOCK_MONOTONIC, &past);
}

bool TimeDelay::delay() {
	bool lag; //is the frame lagging behind the designated framerate?
	clock_gettime(CLOCK_MONOTONIC, &current);
	int elapsed_time = elapsed(past,current);
	int remaining_time = time_delay - elapsed_time;
	if (remaining_time > 0){
		usleep(remaining_time);
		lag=false;
	}else{
		lag=true;
	}
	swap(current,past);
	return lag;
}

inline int TimeDelay::elapsed(timespec start, timespec end) {
	return (int)((end.tv_sec*SPUSec + end.tv_nsec*NPUSec) - (start.tv_sec*SPUSec + start.tv_nsec*NPUSec));
}

void TimeDelay::swap(timespec &a, timespec &b) {
	timespec c = a;
	a = b;
	b = c;
	return;
}

} /* namespace COLA */
