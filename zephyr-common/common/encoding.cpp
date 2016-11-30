/*
 * encoding.cpp
 *
 *  Created on: Mar 8, 2015
 *      Author: Devan
 */

#include "encoding.h"
#include <mbed.h>

/**
 * Decode an encoded message in the Zephyr format, including the final DELIMITER_BYTE.
 * @param buffer Pointer to buffer holding the encoded message
 * @param len Length of buffer.
 * @param p_msg [out] Pointer to blank CANMessage for output
 * @return True on success, false otherwise
 */
bool ZephyrEncoding::decode(const char* buffer, int len, mbed::CANMessage* p_msg) {
	// Sanity check: minimum # bytes of a length-1 message
	if(len < 7) {
		return false;
	}

	// Must end with the delimiter.
	if(buffer[len-1] != ZephyrEncoding::DELIMITER_BYTE) {
		return false;
	}

	// Over the maximum size possible (8-byte msg).
	if(len > ZephyrEncoding::MAX_ENCODED_SIZE) {
		return false;
	}

	char msg_raw[MAX_ENCODED_SIZE];

	// Decode the message, taking into account any escape sequences.
	int pos = 0;
	int pos_trans = 0;
	while(pos_trans < len-1) { // exclude delimiter at the end
		char b = buffer[pos_trans];
		pos_trans++;
		// TODO: Sanity check for no DELIMITER_BYTE in the middle?
		if(b == ZephyrEncoding::ESCAPE_BYTE) {
			// Escape byte
			char nextb = buffer[pos_trans];
			pos_trans++;
			if(nextb == ZephyrEncoding::ESCAPE_BYTE) { // escape+escape=escape
				// nothing to do (b=ESCAPE_BYTE already)
			} else if(nextb == 'd') { // escape+'d'=delimiter
				b = ZephyrEncoding::DELIMITER_BYTE;
			} else { // illegal
				// debug(String.format("Illegal escape sequence: escape + 0x%02x", nextb));
				return false;
			}
		}
		msg_raw[pos++] = b;
	}

	// Decode the preamble
	int id = ((int)(msg_raw[1]) << 8) + msg_raw[0];

	// Decode message length
	int len_msg = msg_raw[2];

	// Actual/transmitted message length
	int actual_len = pos - 3; // exclude 3 bytes of preamble

	// Sanity check: does the final message length agree?
	if(actual_len != len_msg) {
		//debug("Final message length is too short - " + actual_len + " vs expected " + len_msg);
		return false;
	}

	p_msg->id = id;
	p_msg->type = CANData;
	p_msg->format = CANStandard;
	p_msg->len = len_msg;
	for(int i = 0; i < len_msg; i++)
		p_msg->data[i] = msg_raw[3+i];

	return true;
}

/**
 * Encodes a CAN message in the Zephyr telemetry wire format.
 * The format is
 * [id (2 bytes)]
 * [len of message in bytes (1 byte)]
 * [msg (little-endian; up to 8 bytes)]
 * [delimiter (1 byte)]
 * Delimiter byte becomes escape+'d'.
 * Escape byte becomes escape+escape.
 * Escape + anything else is illegal.
 * @param msg The CAN message to encode.
 * @param buffer buffer to write encoded format to
 * Usage:
 *   uint8_t buffer[ZephyrEncoding::MAX_ENCODED_SIZE];
 *   int len = ZephyrEncoding::encode(your_msg, buffer);
 *   write_bytes(buffer, len);
 */
int ZephyrEncoding::encode(const mbed::CANMessage& msg, uint8_t* buffer) {
	const uint8_t idLSB = (uint8_t)(msg.id & 0xFF);
	const uint8_t idMSB = (uint8_t)((msg.id >> 8) & 0xFF);

	int pos = 0;
// Enqueue byte into current position and increment transmission length.
#define _ENQUEUE_BYTE(byte) do { \
	if(byte == ZephyrEncoding::DELIMITER_BYTE) { \
		buffer[pos++] = ZephyrEncoding::ESCAPE_BYTE; \
		buffer[pos++] = 'd'; \
	} else if(byte == ZephyrEncoding::ESCAPE_BYTE) { \
		buffer[pos++] = ZephyrEncoding::ESCAPE_BYTE; \
		buffer[pos++] = ZephyrEncoding::ESCAPE_BYTE; \
	} else { \
		buffer[pos++] = byte; \
	} \
} while(0);

	_ENQUEUE_BYTE(idLSB);
	_ENQUEUE_BYTE(idMSB);
	_ENQUEUE_BYTE((uint8_t)(msg.len));
	for(int i = 0; i < msg.len; i++) {
		uint8_t b = msg.data[i];
		_ENQUEUE_BYTE(b);
	}
	// Add a delimiter byte (and ensure it doesn't get escaped)
	buffer[pos++] = ZephyrEncoding::DELIMITER_BYTE;

	return pos;
}

/**
 * Encodes a CAN message in the Impulse telemetry wire format
 * @param msg The CAN message to encode.
 * @param buffer buffer to write encoded format to
 *
 * Usage:
 *   uint8_t buffer[ImpulseEncoding::MAX_ENCODED_SIZE];
 *   int len = ImpulseEncoding::encode(your_msg, buffer);
 *   write_bytes(buffer, len);
 */
int ImpulseEncoding::encode(const mbed::CANMessage& msg, uint8_t* buffer) {
	const uint16_t preamble = (msg.id << 4) | msg.len;
	const uint8_t  preambleLSB = (uint8_t)(preamble & 0xFF);
	const uint8_t  preambleMSB = (uint8_t)((preamble >> 8) & 0xFF);

	int pos = 0;
	buffer[pos++] = START_BYTE;
	if (ImpulseEncoding::isSpecialByte(preambleLSB)) {
		buffer[pos++] = ImpulseEncoding::ESCAPE_BYTE;
		buffer[pos++] = preambleLSB ^ ESCAPE_BYTE;
	} else {
		buffer[pos++] = preambleLSB;
	}

	if (ImpulseEncoding::isSpecialByte(preambleMSB)) {
		buffer[pos++] = ImpulseEncoding::ESCAPE_BYTE;
		buffer[pos++] = preambleMSB ^ ImpulseEncoding::ESCAPE_BYTE;
	} else {
		buffer[pos++] = preambleMSB;
	}

	for (int i=0; i < msg.len; i++) {
		const uint8_t datum = (const uint8_t)(msg.data[i]);
		if (ImpulseEncoding::isSpecialByte(datum)) {
			buffer[pos++] = datum;
		} else {
			buffer[pos++] = ImpulseEncoding::ESCAPE_BYTE;
			buffer[pos++] = datum ^ ImpulseEncoding::ESCAPE_BYTE;
		}
	}

	return pos;
}

