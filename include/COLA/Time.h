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
/**
 * \brief	How COLA views time
 *
 * COLA::Time is a singleton object that makes a best effort at controlling the flow of time inside the program. For now, this is
 * not written with actual real-time kernel usage. Essentially, the object "burns" time when the loop rate exceeds a pre designated
 * FPS. It also contains a flag set when the loop time is above the current fps (the program is lagging).
 *
 * Time has two modes, live and offline. The mode setting affects how time will externally report time. In offline mode, we assume that
 * the data source is a video file. Thus, the time elapsed between frames is independent of the programs run time. Thus, time reports the
 * elapsed time between to frames as the difference in frame number multiplied by the inverse of frames per second. In live mode, the
 * object returns the actual clock difference between the two frames.
 */
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
	/**
	 * @brief	Singleton Constructor/Instance Retrieve
	 *
	 * Since time is a singleton object, the constructor is made private. Instead, we use use the public Instance method to get the
	 * current instantiation, or construct a new instantiation using the supplied arguments. NOTE: if the singleton has already been
	 * Instantiated, the arguments fed into instance are ignored.
	 *
	 * @param fps int the frames per second we want to run at
	 * @param live bool sets whether the object is in real-time or offline mode. True sets to real-time mode False sets to offline.
	 * @return COLA::Time* the pointer to the global singleton instantiation.
	 */
	static COLA::Time* Instance(int fps=DEFAULT_FPS, bool live=false);
	/**
	 * @brief	Returns the elapsed time in seconds
	 *
	 * This function returns the elapsed time in realtime or offline (frame count difference multiplied by inverse FPS) depending on
	 * how the original object was instantiated.
	 *
	 * @param &start COLA::FrameDescriptor the earlier frame
	 * @param &end COLA::FrameDescriptor the later frame
	 * @return float the elapsed time in seconds
	 */
	float timeElapsed(COLA::FrameDescriptor &start, COLA::FrameDescriptor &end);
	/**
	 * @brief	burn off remaining time
	 *
	 * This function burns off any time remaining in the loop (to match requested FPS) and sets isLag to false. If the time is greater, it returns immediatly
	 * with true (lag) and sets isLag to true.
	 * @return bool if true, the program is lagging
	 */
	bool delay(void);
	/**
	 * @brief	sets the timestamp for NOW in a given COLA::FrameDescriptor
	 *
	 * This function updates the frame number (number of frames prior since the beginning of execution) (ALWAYS) and the timestamp
	 * (ONLY IN LIVE MODE)
	 * @param &frame COLA::FrameDescriptor the reference to the frame we want to set the time for.
	 */
	void setTime(COLA::FrameDescriptor &frame);
};

} /* namespace COLA */

#endif /* TIMEDELAY_H_ */
