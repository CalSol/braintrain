#include "NonBlockingUSBSerial.h"

NonBlockingUSBSerial::NonBlockingUSBSerial(uint16_t vendor_id, uint16_t product_id, uint16_t product_release, bool connect_blocking)
    : USBSerial(vendor_id, product_id, product_release, connect_blocking) {

}

bool NonBlockingUSBSerial::writeBlockNB(uint8_t* buf, uint16_t size) {
    if (size <= MAX_PACKET_SIZE_EPBULK && configured()) {
        /* Return true if the data was enqueued successfully
           Enqueued data may be lost if the host disconnects, but data
           will almost certainly be lost in that case anyways */
        return (endpointWrite(EPBULK_IN, buf, size) == EP_PENDING);
    }
    return false;
}
