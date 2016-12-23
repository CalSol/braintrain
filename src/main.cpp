#include "mbed.h"

DigitalOut ledR(P0_5);
DigitalOut ledG(P0_6);
DigitalOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

CAN can(P0_28, P0_29);

int main() {
  ledR = 1;
  ledG = 1;
  ledB = 1;
  led1 = 0;
  led2 = 0;

  Timer lastBtnTime;
  lastBtnTime.start();
  bool lastBtn = 0;
  bool toggle = 0;

  can.frequency(100000);

  ledG = 0;
  wait(0.125);
  ledG = 1;

  while (1) {
    CANMessage msg;
    while (can.read(msg)) {
      led1 = !led1;
    }

    if (btn == 0 && lastBtn == 1) {
      if (lastBtnTime.read_ms() > 10) {
        CANMessage msg;
        msg.format = CANStandard;
        msg.id = 42;
        msg.len = 0;
        can.write(msg);
        toggle = !toggle;
        ledB = !toggle;
        lastBtn = 0;
      }
    } else if (btn == 1 && lastBtn == 0) {
      if (lastBtnTime.read_ms() > 10) {
        lastBtn = 1;
      }
    } else {
      lastBtnTime.reset();
    }

    led2 = !btn;
  }

  // Reset CAN controller
  if (LPC_C_CAN0->CANCNTL & (1 << 0)) {
      LPC_C_CAN0->CANCNTL &= ~(1 << 0);
      ledR = 0;
  }
}
