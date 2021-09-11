#include "mbed.h"

#include "ledutils.h"

DigitalOut led1(LED1);

// Replace pin assignments if different
PwmOut ledR(D10);
PwmOut ledG(D11);
PwmOut ledB(D12);

DigitalIn btn(D13, PullUp);

RawSerial serial(SERIAL_TX, SERIAL_RX, 115200);

int main() {
  while(1) {
  led1.write(1);
  
  ledR.write(0);
  ledG.write(0);
  ledB.write(0);
  
  wait(0.5);
  
  led1.write(0);
  
  ledR.write(1);
  ledG.write(1);
  ledB.write(1);
  }
}
