/*
 * hardware_common_mbed.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: edwardw
 */

#include "hardware_common_mbed.h"

hardware_common_mbed::hardware_common_mbed(Timer* _timer, CAN* _can) {
    p_timer = _timer;
    p_can = _can;
    p_canBuffer = new CANRXTXBuffer<32, 16>(*_can);
}

hardware_common_mbed::~hardware_common_mbed() {
    p_timer = NULL;
    delete p_canBuffer;
    p_canBuffer = NULL;
    p_can = NULL;
}

static CANRXTXBuffer<32, 16>* _handleCANMessageBuffer = NULL;
// handleCANMessage is attached to the CAN RX interrupt.
void _handleCANMessage() {
    _handleCANMessageBuffer->handleIrq();
}

void hardware_common_mbed::setupCAN(void) {
    p_can->frequency(CAN_FREQUENCY);
    _handleCANMessageBuffer = p_canBuffer;
    p_can->attach(_handleCANMessage, CAN::RxIrq);
    p_can->attach(_handleCANMessage, CAN::TxIrq);
}

void hardware_common_mbed::startTimingCommon(TimingCommon* timing) {
    timing->start(p_timer);
}

int hardware_common_mbed::readCANMessage(CANMessage& msg) {
    return p_canBuffer->read(msg) == 1; // read()-->1 = success
}

int hardware_common_mbed::writeCANMessage(CANMessage msg) {
    return p_canBuffer->write(msg) == 1; // write()-->1 = success
}

bool hardware_common_mbed::checkCANController() {
    /*
     * Implementation varies between LPC11C14 and LPC1549.
     */
    return true;
}

void hardware_common_mbed::setupLEDs(void) {
    /*
     * The default implementation is blank since boards have varying numbers
     * of LEDs. Override this method for each specific board to turn off all
     * the LEDs.
     */
}
