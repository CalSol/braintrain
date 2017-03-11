#ifndef SLCAN_H_
#define SLCAN_H_

#include <mbed.h>

class SLCANBase {
public:
    bool update();
    virtual ~SLCANBase();

protected:
    SLCANBase();

    // To be implemented by subclasses
    virtual bool setBaudrate(int baudrate) = 0;
    virtual bool setMode(CAN::Mode mode) = 0;
    virtual bool transmitMessage(const CANMessage& msg) = 0;

    virtual bool processCommands() = 0;
    virtual bool processCANMessages() = 0;
    virtual bool flush() = 0;
    virtual bool inputReadable() const = 0;
    virtual int  readInputByte() = 0;

    virtual uint8_t getFirmwareVersion();
    virtual uint8_t getHardwareVersion();
    virtual const char* getSerialString();

    // Shared amongst subclasses
    static size_t formatCANMessage(const CANMessage& msg, char* buf, size_t max_len);
    static size_t formattedCANMessageLength(const CANMessage& msg);
    static size_t commandResponseLength(const char* command);
    bool execCommand(const char* command, char* response);
    bool readCommand();
    
    bool commandQueued;
    char inputCommandBuffer[32];
    
private:
    bool execConfigCommand(const char* command);
    bool execTransmitCommand(const char* command, char* response);
    bool execDiagnosticCommand(const char *command, char* response);
    
    bool commandOverflow;
    size_t inputCommandLen;
};

#include "usb_slcan.h"

#endif
