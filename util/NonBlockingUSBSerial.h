#ifndef NONBLOCKINGUSBSERIAL_H_
#define NONBLOCKINGUSBSERIAL_H_
#include <stdint.h>
#include <USBSerial.h>

class NonBlockingUSBSerial : public USBSerial {
public:
    /**
    *   Constructor
    *
    * @param vendor_id Your vendor_id (default: 0x1f00)
    * @param product_id Your product_id (default: 0x2012)
    * @param product_release Your product_release (default: 0x0001)
    * @param connect_blocking define if the connection must be blocked if USB not plugged in (default: false)
    *
    */
    NonBlockingUSBSerial(uint16_t vendor_id = 0x1f00, uint16_t product_id = 0x2012, uint16_t product_release = 0x0001, bool connect_blocking = false);

    /**
    * Write a block of data without waiting.
    *
    * For more efficiency, a block of size 64 (maximum size of a bulk endpoint) has to be written.
    *
    * @param buf pointer on data which will be written
    * @param size size of the buffer. The maximum size of a block is limited by the size of the endpoint (64 bytes)
    *
    * @returns true if successful
    */
    bool writeBlockNB(uint8_t* buf, uint16_t size);
};

#endif
