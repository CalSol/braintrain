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

    virtual uint8_t getFirmwareVersion();
    virtual uint8_t getHardwareVersion();
    virtual const char* getSerialString();

    // Shared amongst subclasses
    static size_t formatCANMessage(const CANMessage& msg, char* buf, size_t max_len);
    static size_t formattedCANMessageLength(const CANMessage& msg);
    static size_t commandResponseLength(const char* command);
    bool execCommand(const char* command, char* response=NULL);

    template <typename StreamType>
    bool readCommand(StreamType& stream);

    bool commandQueued;
    char inputCommandBuffer[32];
    
private:
    bool execConfigCommand(const char* command);
    bool execTransmitCommand(const char* command, char* response);
    bool execDiagnosticCommand(const char *command, char* response);
    
    bool commandOverflow;
    size_t inputCommandLen;
};

/* Read and buffer one command if possible */
/* This really shouldn't need to be templatized, but there's no
   common interface base class between USBSerial and RawSerial... */
template <typename StreamType>
bool SLCANBase::readCommand(StreamType& stream) {
    bool active = false;
    while (!commandQueued && stream.readable()) {
        char c = (char)stream.getc();
        if (c == '\r') {
            if (commandOverflow) {
                // Replace with a dummy invalid command so we return an error
                inputCommandBuffer[0] = '!';
                inputCommandBuffer[1] = '\0';
                inputCommandLen = 0;
                commandOverflow = false;
                active = true;
            } else {
                // Null-terminate the buffered command
                inputCommandBuffer[inputCommandLen] = '\0';
                //stream.puts(inputCommandBuffer);
                inputCommandLen = 0;
                active = true;
            }
            commandQueued = true;
        } else if (c == '\n' && inputCommandLen == 0) {
            // Ignore line feeds immediately after a carriage return
        } else if (commandOverflow) {
            // Swallow the rest of the command when overflow occurs
        } else {
            // Append to the end of the command
            inputCommandBuffer[inputCommandLen++] = c;

            if (inputCommandLen >= sizeof(inputCommandBuffer)) {
                commandOverflow = true;
            }
        }
    }

    return active;
}

#include "serial_slcan.h"
#include "usb_slcan.h"

#endif
