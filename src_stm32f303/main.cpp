#include "mbed.h"

#include "ledutils.h"

DigitalOut led1(LED1);

// Replace pin assignments if different
PwmOut ledR(PA_11);
PwmOut ledG(PB_5);
PwmOut ledB(PB_4);

DigitalIn btn(PA_8, PullUp);

RawSerial serial(SERIAL_TX, SERIAL_RX, 115200);

int main() {
  ledR.write(0);
  ledG.write(0);
  ledB.write(0);
}
