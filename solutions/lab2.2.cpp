#include "mbed.h"

#include "ledutils.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_28, P0_29);

int main() {
  // Start with RGB LED off
  ledR = 1;
  ledG = 1;
  ledB = 1;

  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  // Button edge detection
  bool lastButton = true;

  while (true) {
    bool thisButton = btn;
    if (thisButton != lastButton && btn == 0) {
      can.write(CANMessage(0x42));
    }
    lastButton = thisButton;
  }
}
