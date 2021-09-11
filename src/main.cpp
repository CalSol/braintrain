#include "mbed.h"

#include "ledutils.h"

DigitalOut led1(LED1);

// Replace pin assignments if different
PwmOut ledR(D10);
PwmOut ledG(D11);
PwmOut ledB(D12);

DigitalIn btn(D9, PullUp);

RawSerial serial(SERIAL_TX, SERIAL_RX, 115200);

int main() {
  ledR.write(1);
  ledG.write(1);
  ledB.write(1);
}
