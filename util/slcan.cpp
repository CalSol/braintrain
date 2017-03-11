#include "slcan.h"

// Helper methods for parsing commands
static bool parse_hex_digits(const char* input, uint8_t num_digits, uint32_t* value_out) {
    bool success = true;
    uint32_t value = 0;

    uint8_t i;
    for (i=0; i < num_digits; i++) {
        uint32_t nibble = 0;
        if (input[i] >= '0' && input[i] <= '9') {
            nibble = 0x0 + (input[i] - '0');
        } else if (input[i] >= 'a' && input[i] <= 'f') {
            nibble = 0xA + (input[i] - 'a');
        } else if (input[i] >= 'A' && input[i] <= 'F') {
            nibble = 0xA + (input[i] - 'A');
        } else {
            success = false;
            break;
        }
        uint8_t offset = 4*(num_digits-i-1);
        value |= (nibble << offset);
    }

    if (success) {
        *value_out = value;
    }

    return success;
}

static bool parse_hex_values(const char* input, uint8_t num_values, uint8_t* values_out) {
    uint8_t i;
    for (i=0; i < num_values; i++) {
        uint32_t value;
        if (parse_hex_digits(input, 2, &value)) {
            values_out[i] = (uint8_t)value;
        } else {
            return false;
        }
        input += 2;
    }

    return true;
}

static bool parse_dec_digit(const char* input, uint8_t* value_out) {
    if (input[0] >= '0' && input[0] <= '9') {
        *value_out = 0 + (input[0] - '0');
        return true;
    } else {
        return false;
    }
}

static inline char format_nibble(uint8_t x) {
    uint8_t nibble = x & 0x0F;
    return (nibble < 10) ? ('0' + nibble) : ('A' + (nibble - 10));
}

static inline char format_digit(uint8_t d) {
    return '0' + d;
}

SLCANBase::SLCANBase() {

}

SLCANBase::~SLCANBase() {

}

uint8_t SLCANBase::getFirmwareVersion() {
    // firmware version in BCD
    return 0x10;
}

uint8_t SLCANBase::getHardwareVersion() {
    // hardware version in BCD
    return 0x10;
}

const char* SLCANBase::getSerialString() {
    // 4 character serial number
    return "C254";
}

bool SLCANBase::update() {
    bool active = false;
    if (processCommands()) {
        active = true;
    }
    if (processCANMessages()) {
        active = true;
    }

    if (active) {
        flush();
    }

    return active;
}

size_t SLCANBase::formattedCANMessageLength(const CANMessage& msg) {
    size_t len;
    if (msg.format == CANStandard) {
        len = 1 + 3 + 1 + (2 * msg.len) + 1;
    } else {
        len = 1 + 8 + 1 + (2 * msg.len) + 1;
    }

    return len;
}

size_t SLCANBase::formatCANMessage(const CANMessage& msg, char* buf, size_t max_len) {
    size_t len = formattedCANMessageLength(msg);
    if (len > max_len) {
        return 0;
    }

    if (msg.format == CANStandard) {
        *buf++ = (msg.type == CANData) ? 't' : 'r';
        *buf++ = format_nibble((uint8_t)(msg.id >> 8));
        *buf++ = format_nibble((uint8_t)(msg.id >> 4));
        *buf++ = format_nibble((uint8_t)(msg.id >> 0));
        *buf++ = format_digit(msg.len);
    } else {
        *buf++ = (msg.type == CANData) ? 'T' : 'R';
        *buf++ = format_nibble((uint8_t)(msg.id >> 28));
        *buf++ = format_nibble((uint8_t)(msg.id >> 24));
        *buf++ = format_nibble((uint8_t)(msg.id >> 20));
        *buf++ = format_nibble((uint8_t)(msg.id >> 16));
        *buf++ = format_nibble((uint8_t)(msg.id >> 12));
        *buf++ = format_nibble((uint8_t)(msg.id >>  8));
        *buf++ = format_nibble((uint8_t)(msg.id >>  4));
        *buf++ = format_nibble((uint8_t)(msg.id >>  0));
        *buf++ = format_digit(msg.len);
    }

    for (unsigned char i=0; i < msg.len; i++) {
        *buf++ = format_nibble((uint8_t)(msg.data[i] >> 4));
        *buf++ = format_nibble((uint8_t)(msg.data[i] >> 0));
    }

    *buf++ = '\r';

    return len;
}

size_t SLCANBase::commandResponseLength(const char* command) {
    switch (command[0]) {
        case 'N':
        case 'V': {
            return 6;
        }
        case 'v':
        case 'F': {
            return 4;
        }
        case 'T':
        case 't':
        case 'R':
        case 'r': {
            return 2;
        }
        default: {
            return 1;
        }
    }
}

bool SLCANBase::execCommand(const char* command, char* response) {
    bool success = false;
    if (response) {
        response[0] = '\0';
    }

    switch (command[0]) {
        // Configuration commands
        case 'S':
        case 's':
        case 'O':
        case 'L':
        case 'l':
        case 'C':
        case 'Z':
        case 'M':
        case 'm': {
            success = execConfigCommand(command);
            break;
        }
        // Transmission commands
        case 't':
        case 'T':
        case 'r':
        case 'R': {
            success = execTransmitCommand(command, response);
            break;
        }
        // Diagnostic commands
        case 'V':
        case 'v':
        case 'N':
        case 'W':
        case 'F':
            success = execDiagnosticCommand(command, response);
            break;
        default: {
            success = false;
            break;
        }
    }

    return success;
}

bool SLCANBase::execConfigCommand(const char* command) {
    bool success = false;
    size_t len = strlen(command);

    // Validate command length
    if (command[0] == 'M' || command[0] == 'm') {
        if (len != 9) {
            return false;
        }
    } else if (command[0] == 's') {
        if (!((len == 5) || (len == 7))) {
            return false;
        }
    } else if (command[0] == 'S' || command[0] == 'Z') {
        if (len != 2) {
            return false;
        }
    } else if (len != 1) {
        return false;
    }

    switch (command[0]) {
        case 'S': {
            bool known = true;
            int baudrate;
            switch (command[1]) {
                case '0': baudrate =   10000; break;
                case '1': baudrate =   20000; break;
                case '2': baudrate =   50000; break;
                case '3': baudrate =  100000; break;
                case '4': baudrate =  125000; break;
                case '5': baudrate =  250000; break;
                case '6': baudrate =  500000; break;
                case '7': baudrate =  800000; break;
                case '8': baudrate = 1000000; break;
                default:  known = false; break;
            }

            if (known) {
                success = setBaudrate(baudrate);
            }

            break;
        }
        case 'O': {
            success = setMode(CAN::Normal);
            break;
        }
        case 'L': {
            success = setMode(CAN::Silent);
            break;
        }
        case 'l': {
            success = setMode(CAN::SilentTest);
            break;
        }
        case 'C': {
            success = setMode(CAN::Reset);
            break;
        }
        case 's': {
            // TODO: implement direct BTR control
            success = true;
            break;
        }
        case 'M':
        case 'm': {
            // TODO: implement filtering
            success = true;
            break;
        }
        case 'Z': {
            // TODO: implement timestamping
            success = true;
            break;
        }
        default: {
            success = false;
            break;
        }
    }

    return success;
}

bool SLCANBase::execTransmitCommand(const char* command, char* response) {
    bool success = false;

    size_t len = strlen(command);

    bool validMessage = false;
    CANMessage msg;

    if (command[0] == 't' || command[0] == 'T') {
        msg.type = CANData;
        msg.format = (command[0] == 't') ? CANStandard : CANExtended;
        size_t idLen = msg.format == CANStandard ? 3 : 8;
        if ((len >= idLen + 2) &&
            parse_hex_digits(&command[1], idLen, (uint32_t*)&msg.id) &&
            parse_dec_digit(&command[idLen + 1], &msg.len)) {
            if ((len == idLen + 2 + 2*msg.len) &&
                (msg.len <= 8) &&
                parse_hex_values(&command[idLen + 2], msg.len, msg.data)) {
                validMessage = true;
            }
        }
    } else if (command[0] == 'r' || command[0] == 'R') {
        msg.type = CANRemote;
        msg.format = (command[0] == 'r') ? CANStandard : CANExtended;
        size_t idLen = msg.format == CANStandard ? 3 : 8;
        if ((len == idLen + 2) &&
            parse_hex_digits(&command[1], idLen, (uint32_t*)(&msg.id)) &&
            parse_dec_digit(&command[idLen + 1], &msg.len)) {
            if (msg.len <= 8) {
                validMessage = true;
            }
        }
    }

    if (validMessage) {
        if (command[0] == 'T' || command[0] == 'R') {
            response[0] = 'Z';
            response[1] = '\0';
        } else if (command[0] == 't' || command[0] == 'r') {
            response[0] = 'z';
            response[1] = '\0';
        }
        success = transmitMessage(msg);
    }

    return success;
}

bool SLCANBase::execDiagnosticCommand(const char* command, char* response) {
    bool success = false;
    size_t len = strlen(command);

    // Validate command length
    if (command[0] == 'W') {
        if (len != 5) {
            return false;
        }
    } else if (len != 1) {
        return false;
    }

    if (!response) {
        return false;
    }

    switch (command[0]) {
        case 'V': {
            success = true;
            uint8_t hwVersion = getHardwareVersion();
            uint8_t fwVersion = getFirmwareVersion();

            response[0] = 'V';
            response[1] = format_nibble(hwVersion >> 4);
            response[2] = format_nibble(hwVersion >> 0);
            response[3] = format_nibble(fwVersion >> 4);
            response[4] = format_nibble(fwVersion >> 0);
            response[5] = '\0';
            break;
        }
        case 'v': {
            success = true;
            uint8_t fwVersion = getFirmwareVersion();
            response[0] = 'v';
            response[1] = format_nibble(fwVersion >> 4);
            response[2] = format_nibble(fwVersion >> 0);
            response[3] = '\0';
            break;
        }
        case 'N': {
            success = true;
            const char* serial = getSerialString();
            size_t index = 0;
            response[index++] = 'N';
            for (int i=0; i < 4; i++) {
                char c = serial[i];
                if (c == '\0') {
                    break;
                } else {
                    response[index++] = c;
                }
            }
            response[index] = '\0';
            break;
        }
        case 'F': {
            success = true;
            uint8_t status = 0x00;
            response[0] = 'F';
            response[1] = format_nibble(status >> 4);
            response[2] = format_nibble(status >> 0);
            response[3] = '\0';
        }
        case 'W': {
            // Just swallow the MCP2515 register write command
            success = true;
            break;
        }
        default: {
            success = false;
            break;
        }
    }

    return success;
}

/* Read and buffer one command if possible */
bool SLCANBase::readCommand() {
    bool active = false;
    while (!commandQueued && inputReadable()) {
        char c = (char)readInputByte();
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
            if (inputCommandBuffer[0] != '\0') {
                commandQueued = true;
            }
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
