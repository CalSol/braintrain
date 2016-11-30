/*
 * can_struct.h
 *
 *  Created on: Jun 9, 2014
 *      Author: Devan
 *
 *  Common CAN message payload structs
 */

#ifndef __ZEPHYR_COMMON_CAN_STRUCT_H__
#define __ZEPHYR_COMMON_CAN_STRUCT_H__
#include <stdint.h>
#include <stddef.h>

#ifndef __ZEPHYR_COMMON_NO_MBED__
#include <mbed.h>
#endif

/* Helper template function for constructing CAN messages
 *
 * @param id the message's 11-bit CAN ID
 * @param data struct/value to send
 *
 * @returns
 *   CANMessage with the bytes and DLC of `data`
 *
 * Example usage:
 *   struct ComplexPayload data;
 *   data.foo = 2.0;
 *   data.bar = 0x10;
 *   can.write(makeMessage(CAN_ID_COMPLEX, data));
 */
template <class DataStruct>
CANMessage makeMessage(int id, const DataStruct& data) {
    static_assert(sizeof(DataStruct) <= 8, "Message payload too big");
    return CANMessage(id, reinterpret_cast<const char*>(&data), sizeof(DataStruct));
}

/* Helper template function for unpacking CAN messages
 *
 * @param msg the CANMessage object to unpack
 *
 * @returns
 *   Value of the CANMessage
 * Example usage:
 *   CANMessage msg;
 *   if (can.read(msg)) {
 *     if (msg.id == CAN_ID_COMPLEX && msg.len == sizeof(ComplexPayload)) {
 *       struct ComplexPayload data = unpackMessage(msg);
 *       if (data.foo > 4.0) {
 *         int baz = data.bar | 0x20;
 *       }
 *     }
 *   }
 */
template <typename DataStruct>
DataStruct unpackMessage(CANMessage& msg) {
    static_assert(offsetof(CANMessage, data) % 4 == 0, "Cast dangerous with struct-alignment");
    const char* sideStepStrictAliasing = reinterpret_cast<char*>(msg.data);
    char* sideStepConst = const_cast<char*>(sideStepStrictAliasing);
    return *(reinterpret_cast<DataStruct*>(sideStepConst));
}

struct MPPTData {
	uint16_t arrayVoltage_10mV;
	uint16_t arrayCurrent_mA;
	uint16_t batteryVoltage_10mV;
	uint16_t temperature_10mC;
};

struct pedalPos {
    float accel;
    float brake;
};

// Sent as CAN_BMS_CAR_SHUTDOWN when turning off the car
enum BMSMeasurementStatus {
	NO_PROBLEM,
	LTC6804_UNREACHABLE,
	CELL_UNDERVOLTAGE,
	CELL_OVERVOLTAGE,
	OVERTEMPERATURE,
	OVERCURRENT,
	BATTERY_POWERED,
	OFF_BUTTON_PRESS,
	OVERCHARGE,
	THERMISTORSBOARD_UNREACHABLE
};

/**
 * To be sent out periodically, notifying the rest of the can nodes on Zephyr
 * of the state of BMS.
 */
struct BMSState {
	BMSMeasurementStatus status;
	char flags;
};

#endif /* __ZEPHYR_COMMON_CAN_STRUCT_H__ */
