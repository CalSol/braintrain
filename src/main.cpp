#include "mbed.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC);

int main() {
  ledR.period_us(500);
  ledG.period_us(500);
  ledB.period_us(500);

  ledR = 1;
  ledG = 1;
  ledB = 1;
  led1 = 0;
  led2 = 0;

  wait(0.125);

  // Initialization code here during green-off
  serial.baud(115200);
  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");
  serial.printf("  Core: %u Hz\r\n", SystemCoreClock);
  ledG = 0.5;
  wait(0.125);

  ledG = 1;
  wait(0.125);

  while (true) {
    // Application code here
  }
}
