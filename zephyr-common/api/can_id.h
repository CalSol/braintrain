/* CalSol - UC Berkeley Solar Vehicle Team
 * can_id.h - Zephyr
 * Purpose: Can ID Definitions
 * Author(s): Steven Rhodes
 * Date: Jun 7th 2014
 */

#ifndef __CAN_ID
#define __CAN_ID

#ifndef IMPULSE
#define CAN_FREQUENCY				500000

#else
#error impulse
#define CAN_FREQUENCY				1000000
#endif

// Heartbeats
#define CAN_HEART_BMS				0x040
#define CAN_HEART_CUTOFF			0x041
#define CAN_HEART_MCC_LEFT			0x042
#define CAN_HEART_MCC_RIGHT			0x043
#define CAN_HEART_DASHBOARD			0x044
#define CAN_HEART_POWERHUB_BOTTOM	0x045
#define CAN_HEART_POWERHUB_TOP		0x046
#define CAN_HEART_TELEMETRY			0x047
#define CAN_HEART_LPCTELEMETRY		0x048
#define CAN_HEART_DATALOGGER		0x049
#define CAN_HEART_MPPT(I)           0x060 + I
#define CAN_HEART_DEMO_CODE			0x050

// BMS controls and messages
#define CAN_BMS_CAR_SHUTDOWN        0x30 // Enum, defined in BMS
#define CAN_BMS_CAR_WARNING			0x31
#define CAN_PACK_VOLTAGE			0x123
#define CAN_PACK_CURRENT			0x124
#define CAN_CELL_VOLTAGE(I,J)		(0x130 + (I)*0x10 + (J))
#define CAN_PACK_TEMPERATURE_HIGH	0x125
#define CAN_PACK_TEMPERATURE_LOW	0x126
#define CAN_PACK_TEMPERATURE_AVG	0x127
#define CAN_BMS_BALANCING			0x128
#define CAN_BMS_STATES				0x129
#define CAN_PACK_TEMPERATURE(I)     (0x160 + (I))

// Cutoff board controls and messages
#define CAN_CUTOFF_TRIGGER			0x260
#define CAN_CUTOFF_AIN_VOLTAGES		0x261
#define CAN_CUTOFF_SPI_VOLTAGES		0x262

// Dashboard controls and messages
#define CAN_ACCEL_POS       		0x280
#define CAN_BRAKE_POS       		0x281
#define CAN_PEDAL_POS               0x282

// MCC queries - left
#define CAN_MCC_LEFT_RPM      		0x310

// MCC queries - right
#define CAN_MCC_RIGHT_RPM     		0x311

// MCC thermistors
#define CAN_MCC_LEFT_TEMP			0x320
#define CAN_MCC_RIGHT_TEMP			0x321

#define CAN_MOTOR_OVERHEAT_L		0x322
#define CAN_MOTOR_OVERHEAT_R		0x323

// PowerHub controls and messages
#define CAN_POWERHUBBOTTOM_TURNON	0x500
#define CAN_POWERHUBBOTTOM_TURNOFF	0x501

#define CAN_POWERHUBTOP_TURNON		0x504
#define CAN_POWERHUBTOP_TURNOFF		0x505

// PowerHub current sensors
#define CAN_POWERHUBBOTTOM_CURRENT(I) 0x520 + I

#define CAN_POWERHUBTOP_CURRENT(I) 0x529 + I


#define CAN_MPPT_PWR(I)    0x550 + I
#define CAN_MPPT_VC(I)     0x560 + I
#define CAN_MPPT_T(I)      0x570 + I
#define CAN_MPPT_DATA(I)   0x580 + I

// MPPT controls and messages
// http://goo.gl/KFx2nd
#define CAN_FRONT_RIGHT_MPPT_STATUS	0x600
#define CAN_FRONT_LEFT_MPPT_STATUS	0x601
#define CAN_BACK_RIGHT_MPPT_STATUS	0x602
#define CAN_BACK_LEFT_MPPT_STATUS	0x603
#define CAN_FRONT_RIGHT_MPPT_ENABLE	0x610
#define CAN_FRONT_LEFT_MPPT_ENABLE	0x611
#define CAN_BACK_RIGHT_MPPT_ENABLE	0x612
#define CAN_BACK_LEFT_MPPT_ENABLE	0x613

#define CAN_DASH_SWITCHES			0x700

/** Mostly just tells the current RTC time. */
#define CAN_TELEMETRY_INFO          0x800
#define CAN_TELEMETRY_STAT          0x801
#define CAN_TELEMETRY_RTC           0x802
#define CAN_TELEMETRY_TEMP          0x803

// Telemetry controls and messages

// For inspiration, these are the IDs from Impulse

//// Emergency signals
//#define CAN_EMER_BPS            0x021
//#define CAN_EMER_CUTOFF         0x022
//#define CAN_EMER_DRIVER_IO      0x023
//#define CAN_EMER_DRIVER_CTL     0x024
//#define CAN_EMER_TELEMETRY      0x025
//#define CAN_EMER_OTHER1         0x026
//#define CAN_EMER_OTHER2         0x027
//#define CAN_EMER_OTHER3         0x028
//
//// Heartbeats
//#define CAN_HEART_BPS           0x041
//#define CAN_HEART_CUTOFF        0x042
//#define CAN_HEART_DRIVER_IO     0x043
//#define CAN_HEART_DRIVER_CTL    0x044
//#define CAN_HEART_TELEMETRY     0x045
//#define CAN_HEART_DATALOGGER    0x046
//#define CAN_HEART_OTHER2        0x047
//#define CAN_HEART_OTHER3        0x048
//
//// Cutoff signals
//#define CAN_CUTOFF_VOLT    0x523
//#define CAN_CUTOFF_CURR    0x524
//#define CAN_CUTOFF_NORMAL_SHUTDOWN 0x521
//
//// BPS signals
//#define CAN_BPS_BASE            0x100  // BPS signal base
//#define CAN_BPS_MODULE_OFFSET   0x010  // Difference between modules
//#define CAN_BPS_TEMP_OFFSET     0x00C  // Offset in addition to module offset
//#define CAN_BPS_DIE_TEMP_OFFSET 0x00C  // Offset for LT die temperature
//
//// To Tritium signals
//#define CAN_TRITIUM_DRIVE       0x501
//#define CAN_TRITIUM_RESET       0x503
//
//// From Tritium signals
//#define CAN_TRITIUM_ID          0x400
//#define CAN_TRITIUM_STATUS      0x401
//#define CAN_TRITIUM_BUS         0x402
//#define CAN_TRITIUM_VELOCITY    0x403
//#define CAN_TRITIUM_PHASE_CURR  0x404
//#define CAN_TRITIUM_MOTOR_VOLT  0x405
//#define CAN_TRITIUM_MOTOR_CURR  0x406
//#define CAN_TRITIUM_MOTOR_BEMF  0x407
//#define CAN_TRITIUM_15V_RAIL    0x408
//#define CAN_TRITIUM_LV_RAIL     0x409
//#define CAN_TRITIUM_FAN_SPEED   0x40A
//#define CAN_TRITIUM_MOTOR_TEMP  0x40B
//#define CAN_TRITIUM_AIR_TEMP    0x40C
//#define CAN_TRITIUM_CAP_TEMP    0x40D
//#define CAN_TRITIUM_ODOMETER    0x40E
//
//// Dashboard Signals
//#define CAN_DASHBOARD_INPUTS    0x481

#endif // __CAN_ID
