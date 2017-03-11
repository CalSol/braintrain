#ifndef USB_SLCAN_H_
#define USB_SLCAN_H_

#include "slcan.h"
#include <USBSerial.h>
#include <CircularBuffer.h>
#include <Callback.h>

class USBSLCANBase : public SLCANBase {
protected:
    USBSLCANBase(USBSerial& stream);

    virtual bool inputReadable() const;
    virtual int  readInputByte();
    virtual bool processCommands();
    virtual bool processCANMessages();
    virtual bool flush();

    // To be implemented by subclasses
    virtual bool getNextCANMessage(CANMessage& msg) = 0;
private:
    USBSerial& stream;
    CANMessage queuedMessage;
    bool messageQueued;

    char outputPacketBuffer[64];
    size_t outputPacketLen;
};

class USBSLCANSlave : public USBSLCANBase {
public:
    USBSLCANSlave(USBSerial& stream);

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
