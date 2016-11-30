/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "CalSolSPI.h"

#if DEVICE_SPI

namespace CalSol {

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName _unused) :
        mbed::SPI(mosi, miso, sclk, _unused) {
}

void SPI::transfer(int* values, int len) {
    aquire();
    spi_master_transfer(&_spi, values, len);
}

void SPI::transfer(uint8_t* values, int len) {
    aquire();
    spi_master_transfer_u8(&_spi, values, len);
}

void SPI::transfer(uint16_t* values, int len) {
    aquire();
    spi_master_transfer_u16(&_spi, values, len);
}

void SPI::write_array(const int* values, int len) {
    aquire();
    spi_master_write_array(&_spi, values, len);
}

void SPI::write_array(const uint8_t* values, int len) {
    aquire();
    spi_master_write_array_u8(&_spi, values, len);
}

void SPI::write_array(const uint16_t* values, int len) {
    aquire();
    spi_master_write_array_u16(&_spi, values, len);
}

void SPI::read_array(int* values, int len) {
    aquire();
    spi_master_read_array(&_spi, values, len);
}

void SPI::read_array(uint8_t* values, int len) {
    aquire();
    spi_master_read_array_u8(&_spi, values, len);
}

void SPI::read_array(uint16_t* values, int len) {
    aquire();
    spi_master_read_array_u16(&_spi, values, len);
}

} // namespace CalSol

#endif
