#include "mbed.h"

#include "ledutils.h"

RGBPwmOut rgbLed(P0_5, P0_6, P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_28, P0_29);

int main() {
  // LED Blink State
  Timer ledTimer;  // used to track blink length
  ledTimer.start();

  // Button edge detection
  bool lastButton = true;

  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  while (true) {
    // CAN receive handling
    CANMessage msg;
    while (can.read(msg)) {
      if (msg.id == 0x43) {
        uint16_t hue = (msg.data[0] << 8) | (msg.data[1] << 0);
        rgbLed.hsv_uint16(hue, 65535, 32767);
      } else if (msg.id == 0x42) {
        led2 = 1;
        ledTimer.reset();
      }
    }

    // System actions
    if (ledTimer.read_ms() >= 500) {
      led2 = 0;
    }

    bool thisButton = btn;
    if (thisButton != lastButton && btn == 0) {
      can.write(CANMessage(0x42, NULL, 0));
    }
    lastButton = thisButton;
  }
}
