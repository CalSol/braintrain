#include "mbed.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

int main() {
  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

  while (true) {
    led1 = 1;
    led2 = 0;
    serial.printf("led value is %u\r\n", (int)led1);
    wait(0.5);
    led1 = 0;
    led2 = 1;
    serial.printf("led value is %u\r\n", (int)led1);
    wait(0.5);
  }
}
