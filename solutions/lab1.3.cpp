#include "mbed.h"

#include "ledutils.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC);

int main() {
  while (true) {
    led1 = 1;
    led2 = 0;
    wait(0.5);
    while(!btn);
    led1 = 0;
    led2 = 1;
    wait(0.5);
    while(!btn);
  }
}
