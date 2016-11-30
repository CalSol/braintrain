/*
 * hardware_common_mbed.h
 * mbed-based implementation of hardware_common.
 * Compare to unit testing implementation of hardware_common.
 *
 *  Created on: Jan 10, 2016
 *      Author: edwardw
 */

#ifndef API_HARDWARE_COMMON_MBED_H_
#define API_HARDWARE_COMMON_MBED_H_

#include <mbed.h>
#include <can_buffer.h>
#include <hardware_common.h>

class hardware_common_mbed : public hardware_common {
public:
    /**
     * Create a new instance of mbed implementation of hardware_common.
     * @param _timer Pointer to Timer object (required)
     * @param _can Pointer to CAN object, or null if this board does not use CAN
     */
    hardware_common_mbed(Timer* _timer, CAN* _can = NULL);
    virtual ~hardware_common_mbed();

    /** Helper function to handle a CAN message interrupt. */
    void handleCANMessage();

    virtual void setupCAN(void);
    virtual void startTimingCommon(TimingCommon* timing);
    virtual int readCANMessage(CANMessage& msg);
    virtual int writeCANMessage(CANMessage msg);
    virtual bool checkCANController(void);
    virtual void setupLEDs(void);

private:
    // 32-message buffer.
    CANRXTXBuffer<32, 16>* p_canBuffer;

    CAN* p_can;
    Timer* p_timer;
};

#endif /* API_HARDWARE_COMMON_MBED_H_ */
