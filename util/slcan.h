/* SLCAN/CAN-USB protocol compatible CAN debugging interface */

#ifndef SLCAN_H_
#define SLCAN_H_

#include <mbed.h>
#include <CircularBuffer.h>
#include <Callback.h>

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

    virtual uint8_t getFirmwareVersion();
    virtual uint8_t getHardwareVersion();
    virtual const char* getSerialString();

    // Shared amongst subclasses
    static size_t formatCANMessage(const CANMessage& msg, char* buf, size_t max_len);
    static size_t formattedCANMessageLength(const CANMessage& msg);
    static size_t commandResponseLength(const char* command);
    bool execCommand(const char* command, char* response=NULL);

private:
    bool execConfigCommand(const char* command);
    bool execTransmitCommand(const char* command, char* response);
    bool execDiagnosticCommand(const char *command, char* response);
};

class SerialSLCANBase : public SLCANBase {
protected:
    SerialSLCANBase(RawSerial& stream);
    virtual bool setBaudrate(int baudrate) = 0;
    virtual bool setMode(CAN::Mode mode) = 0;
    virtual bool transmitMessage(const CANMessage& msg) = 0;

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
