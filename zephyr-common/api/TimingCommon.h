/*
 * TimingCommon.h
 *
 * A class for handling common timing functions.
 * Only handles Timer overflow after 30 minutes for now.
 */

#ifndef TIMINGCOMMON_H_
#define TIMINGCOMMON_H_

#ifdef _TESTING_
class Timer;
#else
#include <mbed.h>
#endif

// Number of possible timing-based callbacks
#define TIMING_NUM_CALLBACKS 12

class TimingCommon {
public:
	TimingCommon();
	virtual ~TimingCommon();

	/**
	 * Start timing.
	 * Will start the time with timer.start() if not already started.
	 * @param timer Timer to use.
	 */
	void start(Timer* t);

	/**
	 * Runs each time at the top of the board main run while(1).
	 * @param outOverflow
	 * 	Will be set true on timer overflow, false otherwise.
	 * 	Can be NULL if unused.
	 * @return Current time now (us)
	 */
	int onTick(bool* outOverflow);

	/**
	 * Add a callback to be triggered periodically during onTick().
	 * @param threshold Period of callback (us)
	 * @param callback Callback function
	 * @return 0 on success, -1 on failure (too many callbacks added already)
	 */
	int addCallback(int threshold, void (*callback)(void));

	/**
	 * Add a callback for heartbeat messages to be triggered periodically.
	 * @param threshold Period of heartbeats (us)
	 * @param callback Callback function taking an int (current time)
	 */
	void setHeartbeatCallback(int threshold, void (*callback)(int));
private:
	int lastTickTime;
	Timer* timer;

	int thresholds[TIMING_NUM_CALLBACKS];
	int lastTick[TIMING_NUM_CALLBACKS];
	void (*callbacks[TIMING_NUM_CALLBACKS])(void);
	int num_callbacks;

	int heartbeat_threshold;
	int heartbeat_lastTick;
	void (*heartbeat_callback)(int);

	/**
	 * Reset all callback last tick times to the given time.
	 */
	void resetCallbackLastTicks(int now);
};

#endif /* TIMINGCOMMON_H_ */
