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

#include <COLA/settings.h>

namespace COLA {

class Time {
private:
	timespec current, past;
	float time_delayS, time_delayNS;
	bool live;

	static Time* _instance;
protected:
	Time(int fps, bool live);
public:
	bool isLag;
	static COLA::Time* Instance(int fps=DEFAULT_FPS, bool live=false);
	float timeElapsed(timespec start, timespec end);
	bool delay(void);
};

} /* namespace COLA */

#endif /* TIMEDELAY_H_ */
