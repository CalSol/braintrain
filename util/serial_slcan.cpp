#include "serial_slcan.h"

/* Base class for processing and responding to SLCAN commands over serial */
SerialSLCANBase::SerialSLCANBase(RawSerial& stream)
    : stream(stream),
      commandQueued(false),
      commandOverflow(false),
      inputCommandLen(0) {
}

/* Parse and execute a single SLCAN command and enqueue the response */
bool SerialSLCANBase::processCommands() {
    // Buffer an entire command
    bool active = readCommand(stream);

    // Process the current command
    if (commandQueued) {
        char outputResponseBuffer[32];
        outputResponseBuffer[0] = '\0';
        if (execCommand(inputCommandBuffer, outputResponseBuffer)) {
            // Success
            stream.puts(outputResponseBuffer);
            stream.putc('\r');
        } else {
            // Failure
            stream.putc('\a');
        }
        commandQueued = false;
        active = true;
    }

    return active;
}

/* Read and enqueue as many received CAN messages as will fit */
bool SerialSLCANBase::processCANMessages() {
    bool active = false;
    CANMessage msg;
    while (getNextCANMessage(msg)) {
        char buffer[32];
        size_t len = formatCANMessage(msg, buffer, sizeof(buffer));
        buffer[len] = '\0';
        stream.puts(buffer);
        active = true;
    }

    return active;
}

/* Attempt to transmit the output queue */
bool SerialSLCANBase::flush() {
    return false;
}

/* An SLCAN implementation that directly accesses and controls the CAN peripheral */
SerialSLCANMaster::SerialSLCANMaster(RawSerial& stream, CAN& can)
    : SerialSLCANBase(stream),
      can(can) {
}

bool SerialSLCANMaster::setBaudrate(int baudrate) {
    return (can.frequency(baudrate) == 1);
}

bool SerialSLCANMaster::setMode(CAN::Mode mode) {
    return (can.mode(mode) == 1);
}

bool SerialSLCANMaster::transmitMessage(const CANMessage& msg) {
    return (can.write(msg) == 1);
}

bool SerialSLCANMaster::getNextCANMessage(CANMessage& msg) {
    return (can.read(msg) == 1);
}

/* an SLCAN implementation that only accesses the CAN peripheral through callbacks */
SerialSLCANSlave::SerialSLCANSlave(RawSerial& stream)
    : SerialSLCANBase(stream),
      ignoreConfigCommands(false) {
}

/* Configure SLCAN to silently discard mode/baudrate commands */
void SerialSLCANSlave::setIgnoreConfigCommands(bool ignore) {
    ignoreConfigCommands = ignore;
}

/* Register the handler to change the CAN bitrate on request */
void SerialSLCANSlave::setBaudrateHandler(Callback<bool(int baudrate)> callback) {
    cbSetBaudrate.attach(callback);
}

/* Register the handler to change the CAN mode on request */
void SerialSLCANSlave::setModeHandler(Callback<bool(CAN::Mode mode)> callback) {
    cbSetMode.attach(callback);
}

/* Register the handler to send a CAN message on request */
void SerialSLCANSlave::setTransmitHandler(Callback<bool(const CANMessage& msg)> callback) {
    cbTransmitMessage.attach(callback);
}

bool SerialSLCANSlave::setBaudrate(int baudrate) {
    if (ignoreConfigCommands) {
        return true;
    } else {
        return cbSetBaudrate.call(baudrate);
    }
}

bool SerialSLCANSlave::setMode(CAN::Mode mode) {
    if (ignoreConfigCommands) {
        return true;
    } else {
        return cbSetMode.call(mode);
    }
}

bool SerialSLCANSlave::transmitMessage(const CANMessage& msg) {
    return cbTransmitMessage.call(msg);
}

bool SerialSLCANSlave::putCANMessage(const CANMessage& msg) {
    if (!messageBuffer.full()) {
        messageBuffer.push(msg);
        return true;
    }
    return false;
}

bool SerialSLCANSlave::getNextCANMessage(CANMessage& msg) {
    return messageBuffer.pop(msg);
}
