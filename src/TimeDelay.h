/*
 * TimeDelay.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#ifndef TIMEDELAY_H_
#define TIMEDELAY_H_

#include <time.h>
#include <unistd.h>

namespace COLA {

class TimeDelay {
private:
	timespec current;
	timespec past;
	float time_delay;
	void swap(timespec &a, timespec &b);
public:
	TimeDelay(int fps);
	static float timeElapsedS(timespec start, timespec end);
	static int timeElapsedU(timespec start, timespec end);
	bool delay(void);
};

} /* namespace COLA */

#endif /* TIMEDELAY_H_ */
