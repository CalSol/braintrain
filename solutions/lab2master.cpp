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

  // LED Blink State
  int32_t blinkLengthMs;
  Timer ledTimer;  // used to track blink length
  ledTimer.start();

  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  while (true) {
    // CAN receive handling
    CANMessage msg;
    while (can.read(msg)) {
      if (msg.id == 0x42) {
        led2 = 1;
        blinkLengthMs = 250;
        ledTimer.reset();
      }
    }

    // System actions
    if (ledTimer.read_ms() >= blinkLengthMs) {
      led2 = 0;
    }
  }
}
