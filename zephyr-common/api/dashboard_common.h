/*
 * dashboard_common.h
 *
 *  Created on: Jun 7, 2016
 *      Author: zhong
 */

#ifndef ZEPHYR_COMMON_API_DASHBOARD_COMMON_H_
#define ZEPHYR_COMMON_API_DASHBOARD_COMMON_H_
/** Dashboard switches. Schema is LSB to MSB here:
 * 0 HV (Dash)
 * 1 Solar (Dash)
 * 2 Lights (Dash)
 * 3 Extra (Dash)
 * 4 Forward Toggle (SidePanel)
 * 5 Neutral Toggle (SidePanel)
 * 6 Reverse Toggle (SidePanel)
 * 7 Regen Toggle (SidePanel)
 * 8 Hazards Toggle (SidePanel)
 * 9 Power/Eco Mode (SidePanel)
 * 10 Right Turn Switch (Steering Wheel)
 * 11 Left Turn Switch (Steering Wheel)
 * 12 Horn Switch (Steering Wheel)
 * 13 Blue Switch (Steering Wheel)
 * 14 Cruise Toggle Switch (Steering Wheel)
 * 15 Yellow Switch (Steering Wheel)
 */

enum DashCanIndex {
	CAN_SOLAR,
	CAN_CHARGE,
	CAN_HEADLIGHTS,
	CAN_EXTRA,
	CAN_FORWARD,
	CAN_NEUTRAL,
	CAN_REVERSE,
	CAN_REGEN,
	CAN_HAZARD,
	_CAN_NOT_USED,
	CAN_TURN_RIGHT,
	CAN_TURN_LEFT,
	CAN_HORN,
	CAN_CRUISE_TOGGLE,
	CAN_CRUISE_PLUS,
	CAN_CRUISE_MINUS
};

/** The positions of bits in the uint16_t that CAN_DASH_SWITCHES sends out.
 *  Naming self explanatory */
const int HAZARD_BIT = (1 << CAN_HAZARD);
const int HEADLIGHTS_BIT = (1 << CAN_HEADLIGHTS);
const int CHARGE_BIT = (1 << CAN_CHARGE);
const int SOLAR_BIT = (1 << CAN_SOLAR);
const int FORWARD_BIT = (1 << CAN_FORWARD);
const int NEUTRAL_BIT = (1 << CAN_NEUTRAL);
const int REVERSE_BIT = (1 << CAN_REVERSE);
const int REGEN_BIT = (1 << CAN_REGEN);
const int CRUISE_PLUS_BIT = (1 << CAN_CRUISE_PLUS);
const int CRUISE_MINUS_BIT = (1 << 	CAN_CRUISE_MINUS);
const int TURN_RIGHT_BIT = (1 << CAN_TURN_RIGHT);
const int TURN_LEFT_BIT = (1 << CAN_TURN_LEFT);
const int HORN_BIT = (1 << CAN_HORN);
//const int CONFIRM_BIT = (1 << CAN_CONFIRM);
const int CRUISE_TOGGLE_BIT = (1 << CAN_CRUISE_TOGGLE);
//const int PUSH_TO_TALK_BIT = (1 << CAN_PUSH_TO_TALK);

#endif /* ZEPHYR_COMMON_API_DASHBOARD_COMMON_H_ */
