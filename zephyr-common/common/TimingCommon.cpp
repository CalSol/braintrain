/*
 * TimingCommon.cpp
 */

#include "TimingCommon.h"

TimingCommon::TimingCommon() {
	timer = NULL;
	num_callbacks = 0;
	for(int i = 0; i < TIMING_NUM_CALLBACKS; i++) {
		callbacks[i] = NULL;
		thresholds[i] = -1;
		lastTick[i] = 0;
	}
	heartbeat_threshold = -1;
	heartbeat_callback = NULL;
}

TimingCommon::~TimingCommon() {
}

void TimingCommon::resetCallbackLastTicks(int now) {
	for(int i = 0; i < num_callbacks; i++) {
		lastTick[i] = now;
	}
	heartbeat_lastTick = now;
}

void TimingCommon::start(Timer* t) {
	timer = t;
	timer->start();
	lastTickTime = timer->read_us();
	resetCallbackLastTicks(lastTickTime);
}

int TimingCommon::onTick(bool* outOverflow) {
	int now = timer->read_us();

	// Check if the clock has wrapped
	if (now < lastTickTime) {
		lastTickTime = now;
		if(outOverflow != NULL)
			*outOverflow = true;
		resetCallbackLastTicks(now);
	} else {
		if(outOverflow != NULL)
			*outOverflow = false;

		// Trigger heartbeat callback
		if(heartbeat_callback && now - heartbeat_lastTick > heartbeat_threshold) {
			heartbeat_lastTick = now;
			heartbeat_callback(now);
		}

		// Trigger callbacks, if any
		for(int i = 0; i < num_callbacks; i++) {
			// If threshold has passed for callback
			if(now - lastTick[i] > thresholds[i]) {
				lastTick[i] = now;
				(callbacks[i])();
			}
		}
	}

	return now;
}

int TimingCommon::addCallback(int threshold, void (*callback)(void)) {
	if(num_callbacks >= TIMING_NUM_CALLBACKS) // no more callbacks available
		return -1;

	// Add new callback
	thresholds[num_callbacks] = threshold;
	callbacks[num_callbacks] = callback;
	num_callbacks++;
	return 0;
}

void TimingCommon::setHeartbeatCallback(int threshold, void (*callback)(int)) {
	heartbeat_threshold = threshold;
	heartbeat_callback = callback;
}
