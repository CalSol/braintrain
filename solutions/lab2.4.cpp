#include "mbed.h"

#include "ledutils.h"

RGBPwmOut rgbLed(P0_5, P0_6, P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_28, P0_29);

int main() {
  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  while (true) {
    CANMessage msg;
    while (can.read(msg)) {
      if (msg.id == 0x43) {
        uint16_t hue = (msg.data[0] << 8) | (msg.data[1] << 0);
        rgbLed.hsv_uint16(hue, 65535, 32767);
      }
    }
  }
}
