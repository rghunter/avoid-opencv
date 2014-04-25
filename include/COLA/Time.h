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
#include <utility>

#include <COLA/settings.h>
#include <COLA/FrameDescriptor.h>

namespace COLA {

class Time {
private:
	timespec current, past;
	float time_delayS, time_delayNS;
	bool live;
	int frames;

	static Time* _instance;
protected:
	Time(int fps, bool live);
public:
	bool isLag;
	static COLA::Time* Instance(int fps=DEFAULT_FPS, bool live=false);
	float timeElapsed(COLA::FrameDescriptor &start, COLA::FrameDescriptor &end);
	bool delay(void);
	void setTime(COLA::FrameDescriptor &frame);
};

} /* namespace COLA */

#endif /* TIMEDELAY_H_ */
