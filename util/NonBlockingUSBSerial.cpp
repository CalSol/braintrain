#include "NonBlockingUSBSerial.h"

NonBlockingUSBSerial::NonBlockingUSBSerial(uint16_t vendor_id, uint16_t product_id, uint16_t product_release, bool connect_blocking)
    : USBSerial(vendor_id, product_id, product_release, connect_blocking) {

}

bool
NonBlockingUSBSerial::writeBlockNB(uint8_t* buf, uint16_t size) {
    if (size <= MAX_PACKET_SIZE_EPBULK) {
        return USBDevice::writeNB(EPBULK_IN, buf, size, MAX_PACKET_SIZE_EPBULK);
    }
    return false;
}
