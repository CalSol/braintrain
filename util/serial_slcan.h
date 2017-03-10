/* SLCAN/CAN-USB protocol compatible CAN debugging interface */

#ifndef SERIAL_SLCAN_H_
#define SERIAL_SLCAN_H_

#include "slcan.h"
#include <CircularBuffer.h>
#include <Callback.h>

class SerialSLCANBase : public SLCANBase {
protected:
    SerialSLCANBase(RawSerial& stream);

    virtual bool processCommands();
    virtual bool processCANMessages();
    virtual bool flush();

    // To be implemented by subclasses
    virtual bool getNextCANMessage(CANMessage& msg) = 0;
private:
    RawSerial& stream;
    bool commandQueued;
    bool commandOverflow;
    char inputCommandBuffer[32];
    size_t inputCommandLen;
};

class SerialSLCANMaster : public SerialSLCANBase {
public:
    SerialSLCANMaster(RawSerial& stream, CAN& can);

protected:
    virtual bool setBaudrate(int baudrate);
    virtual bool setMode(CAN::Mode mode);
    virtual bool transmitMessage(const CANMessage& msg);
    virtual bool getNextCANMessage(CANMessage& msg);
private:
    CAN& can;
};

class SerialSLCANSlave : public SerialSLCANBase {
public:
    SerialSLCANSlave(RawSerial& stream);

    bool putCANMessage(const CANMessage& msg);
    void setIgnoreConfigCommands(bool ignore);
    void setBaudrateHandler(Callback<bool(int baudrate)> callback);
    void setModeHandler(Callback<bool(CAN::Mode mode)> callback);
    void setTransmitHandler(Callback<bool(const CANMessage& msg)> callback);
protected:
    virtual bool setBaudrate(int baudrate);
    virtual bool setMode(CAN::Mode mode);
    virtual bool transmitMessage(const CANMessage& msg);
    virtual bool getNextCANMessage(CANMessage& msg);
private:
    CircularBuffer<CANMessage, 8, uint8_t> messageBuffer;
    Callback<bool(int baudrate)> cbSetBaudrate;
    Callback<bool(CAN::Mode mode)> cbSetMode;
    Callback<bool(const CANMessage& msg)> cbTransmitMessage;
    bool ignoreConfigCommands;
};

#endif
