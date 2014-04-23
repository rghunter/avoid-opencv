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
	int elapsed_time = timeElapsedU(past,current);
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
void TimeDelay::swap(timespec &a, timespec &b) {
	timespec c = a;
	a = b;
	b = c;
	return;
}

float TimeDelay::timeElapsedS(timespec start, timespec end) {
	return (end.tv_sec + end.tv_nsec/1000000000.0) - (start.tv_sec + start.tv_nsec/1000000000.0);
}

int TimeDelay::timeElapsedU(timespec start, timespec end) {
	return (int)((end.tv_sec*SPUSec + end.tv_nsec*NPUSec) - (start.tv_sec*SPUSec + start.tv_nsec*NPUSec));
}


} /* namespace COLA */
