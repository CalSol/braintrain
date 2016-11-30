/**
 * LTC236x.h
 * For the LTC2365/66 voltage sensor chips.
 *
 */

#ifndef __LTC236x_H__
#define __LTC236x_H__

#include <mbed.h>

class LTC236x {
public:
    LTC236x(SPI& spi, DigitalOut& CS);

    bool requestConversion(uint16_t& data);

private:
    SPI& spi;
    DigitalOut& CS;
};

#endif /* __LTC236x_H__ */
