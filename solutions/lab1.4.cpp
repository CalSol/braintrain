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
  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

  while (true) {
    /* YOUR CODE HERE */
    led1 = 1;
    serial.printf("led1=%d\r\n", (int)led1);
    wait(0.5);
    led1 = 0;
    serial.printf("led1=%d\r\n", (int)led1);
    wait(0.5);
  }
}
