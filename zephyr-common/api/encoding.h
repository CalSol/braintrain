/*
 * encoding.h
 *
 *  Created on: Mar 8, 2015
 *      Author: Devan
 */

#ifndef ENCODING_H_
#define ENCODING_H_

#include <stdint.h>
namespace mbed {
	class CANMessage;
}

namespace ZephyrEncoding {
	// Worst-case encoded-size:
	// 12*2 escaped data bytes + delimiter
	const int MAX_ENCODED_SIZE = 25;

	// Escape byte used to escape DELIMITER_BYTE.
	// If this byte occurs, replace it with two of itself.
	const uint8_t ESCAPE_BYTE = 0xAA;

	// Message delimiter byte ('\n')
	const uint8_t DELIMITER_BYTE = '\n';

	bool decode(const char* buffer, int len, mbed::CANMessage* p_msg);
	int encode(const mbed::CANMessage& msg, uint8_t* buffer);
}

namespace ImpulseEncoding {
	// Worst-case encoded-size:
	// Every possible byte that can be escaped gets escaped
    // That means 2 x ([2 preamble bytes] + [8 data bytes]) = 20 bytes
	// Plus one start byte gives 21 bytes
	const int MAX_ENCODED_SIZE = 21;

	const uint8_t ESCAPE_BYTE = 0x75;
	const uint8_t START_BYTE = 0xE7;

	int encode(const mbed::CANMessage& msg, uint8_t* buffer);

	static inline bool isSpecialByte(const uint8_t c) {
		return (c == ImpulseEncoding::START_BYTE || c == ImpulseEncoding::ESCAPE_BYTE);
	}
}


#endif /* ENCODING_H_ */
