/*
 * TimeDelay.h
 *
 *  Created on: Apr 22, 2014
 *      Author: rhunter
 */

#ifndef TIMEDELAY_H_
#define TIMEDELAY_H_

namespace COLA {

class TimeDelay {
private:
	timespec current;
	timespec past;
	float time_delay;
	void swap(void *a, void *b);
public:
	TimeDelay(int fps);
	virtual ~TimeDelay();
	void delay(void);
};

} /* namespace COLA */

#endif /* TIMEDELAY_H_ */
