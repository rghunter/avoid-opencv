/*
 * TimeDelay.cpp
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#include "TimeDelay.h"

#define NPS 1000000000.0

namespace COLA {

TimeDelay::TimeDelay(int fps) {
	time_delay = (1.0/(float)fps)*NPS;
	clock_gettime(CLOCK_MONOTONIC, past);
}

TimeDelay::~TimeDelay() {
	// TODO Auto-generated destructor stub
}

void TimeDelay::delay() {
	clock_gettime(CLOCK_MONOTONIC, current);

}

inline int elapsed(timespec start, timespec end) {
	return (int)((end.tv_sec*NPS + end.tv_nsec) - (start.tv_sec*NPS + start.tv_nsec));
}

void TimeDelay::swap(void &a, void &b) {
	void *c = a;
	a = b;
	b = c;
	return;
}

} /* namespace COLA */
