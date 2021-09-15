#include "mbed.h"

#include "ledutils.h"

DigitalOut led1(LED1);

RawSerial serial(SERIAL_TX, SERIAL_RX, 115200);

// Replace pin assignments if different
PwmOut ledR(D9);
PwmOut ledG(D11);
PwmOut ledB(D12);

DigitalIn btn(D8, PullUp);

int main() {
  /* YOUR INITIALIZATION HERE */
  while (true) {
    /* YOUR CODE HERE */
    if (btn) {
      led1 = 1;
      wait(0.5);
      led1 = 0;
      wait(0.5);
    }
  }
}
