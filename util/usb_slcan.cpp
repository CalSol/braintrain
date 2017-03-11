#include "slcan.h"

USBSLCANBase::USBSLCANBase(USBSerial& stream)
    : stream(stream),
      messageQueued(false),
      outputPacketLen(0) {
    
}

/* Parse and execute a single SLCAN command and enqueue the response */
bool USBSLCANBase::processCommands() {
    // Buffer an entire command
    bool active = readCommand(stream);

    // Process the current command if there's space to send the response
    if (commandQueued) {
        size_t responseLength = commandResponseLength(inputCommandBuffer);
        if ((outputPacketLen + responseLength) <= sizeof(outputPacketBuffer)) {
            char outputResponseBuffer[32];
            outputResponseBuffer[0] = '\0';
            if (execCommand(inputCommandBuffer, outputResponseBuffer)) {
                // Success
                for (char* s = outputResponseBuffer; *s != '\0'; s++) {
                    outputPacketBuffer[outputPacketLen++] = *s;
                }
                outputPacketBuffer[outputPacketLen++] = '\r';
            } else {
                // Failure
                outputPacketBuffer[outputPacketLen++] = '\a';
            }
            commandQueued = false;
            active = true;
        }
    }
    
    return active;
}

/* Read and enqueue as many received CAN messages as will fit */
bool USBSLCANBase::processCANMessages() {
    bool active = false;
    
    size_t bytesAvailable = sizeof(outputPacketBuffer) - outputPacketLen;
    char* packetTail = &outputPacketBuffer[outputPacketLen];
    
    if (messageQueued) {
        size_t bytesConsumed = formatCANMessage(queuedMessage, packetTail, bytesAvailable);
        if (bytesConsumed > 0) {
            active = true;
            messageQueued = false;
            bytesAvailable -= bytesConsumed;
            packetTail += bytesConsumed;
            outputPacketLen += bytesConsumed;
        }
    }
    
    if (!messageQueued) {
        while (getNextCANMessage(queuedMessage)) {
            size_t bytesConsumed = formatCANMessage(queuedMessage, packetTail, bytesAvailable);
            if (bytesConsumed > 0) {
                active = true;
                bytesAvailable -= bytesConsumed;
                packetTail += bytesConsumed;
                outputPacketLen += bytesConsumed;
            } else {
                messageQueued = true;
                break;
            }
        }
    }
    
    return active;
}

/* Attempt to transmit the output queue */
bool USBSLCANBase::flush() {
    bool active = false;
    if (outputPacketLen > 0) {
        bool sent = stream.writeBlock((uint8_t*)(outputPacketBuffer),
                                        (uint16_t)(outputPacketLen));
        if (sent) {
            active = true;
            outputPacketLen = 0;
        }
    }
    return active;
}

/* an SLCAN implementation that only accesses the CAN peripheral through callbacks */
USBSLCANSlave::USBSLCANSlave(USBSerial& stream)
    : USBSLCANBase(stream),
      ignoreConfigCommands(false) {
}

/* Configure SLCAN to silently discard mode/baudrate commands */
void USBSLCANSlave::setIgnoreConfigCommands(bool ignore) {
    ignoreConfigCommands = ignore;
}

/* Register the handler to change the CAN bitrate on request */
void USBSLCANSlave::setBaudrateHandler(Callback<bool(int baudrate)> callback) {
    cbSetBaudrate.attach(callback);
}

/* Register the handler to change the CAN mode on request */
void USBSLCANSlave::setModeHandler(Callback<bool(CAN::Mode mode)> callback) {
    cbSetMode.attach(callback);
}

/* Register the handler to send a CAN message on request */
void USBSLCANSlave::setTransmitHandler(Callback<bool(const CANMessage& msg)> callback) {
    cbTransmitMessage.attach(callback);
}

bool USBSLCANSlave::setBaudrate(int baudrate) {
    if (ignoreConfigCommands) {
        return true;
    } else {
        return cbSetBaudrate.call(baudrate);
    }
}

bool USBSLCANSlave::setMode(CAN::Mode mode) {
    if (ignoreConfigCommands) {
        return true;
    } else {
        return cbSetMode.call(mode);
    }
}

bool USBSLCANSlave::transmitMessage(const CANMessage& msg) {
    return cbTransmitMessage.call(msg);
}

bool USBSLCANSlave::putCANMessage(const CANMessage& msg) {
    if (!messageBuffer.full()) {
        messageBuffer.push(msg);
        return true;
    }
    return false;
}

bool USBSLCANSlave::getNextCANMessage(CANMessage& msg) {
    return messageBuffer.pop(msg);
}
