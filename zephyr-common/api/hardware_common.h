/*
 * hardware_common.h
 * Common hardware interaction functionalities, including CAN, LEDs, etc.
 *
 *  Created on: Jan 10, 2016
 *      Author: edwardw
 */

#ifndef API_HARDWARE_COMMON_H_
#define API_HARDWARE_COMMON_H_

#include <can_id.h>
#include <TimingCommon.h>

// Note: for CAN applications, include either CAN.h (HW) or can_lite.h (SW)

/**
 * Representation of common board interfaces.
 */
class hardware_common {
public:
    hardware_common() {};
    virtual ~hardware_common() {};
    /**
     * Set up the CAN controller.
     */
    virtual void setupCAN(void) = 0;

    /**
     * Start timing functionality.
     */
    virtual void startTimingCommon(TimingCommon* timing) = 0;

    /**
     * Read a CANMessage from the board.
     *
     * @param msg CANMessage to be written
     * @return 1 on success, 0 on failure
     */
    virtual int readCANMessage(CANMessage& msg) = 0;

    /**
     * Write a CANMessage to the board.
     *
     * @param msg A CANMessage to write.
     * @return 1 on success, 0 on failure
     */
    virtual int writeCANMessage(CANMessage msg) = 0;

    /**
     * Check if the CAN controller is alive or not. If it isn't, reset the
     * controller.
     * @return True if the CAN controller was alive; false if it was not and
     *         needed to be reset.
     */
    virtual bool checkCANController(void) = 0;

    /**
     * Set up LEDs (initialize them to all off).
     */
    virtual void setupLEDs(void) = 0;
};

#endif /* API_HARDWARE_COMMON_H_ */
