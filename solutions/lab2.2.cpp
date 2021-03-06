#include "mbed.h"

#include "ledutils.h"

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_28, P0_29);

int main() {
  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  // Button edge detection
  bool lastButton = true;

  while (true) {
    bool thisButton = btn;
    if (thisButton != lastButton && btn == 0) {
      can.write(CANMessage(0x42, NULL, 0));
    }
    lastButton = thisButton;
  }
}
