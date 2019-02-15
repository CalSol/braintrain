#include "mbed.h"

#include "ledutils.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

int main() {
  led1 = 0;
  led2 = 0;


  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

  ledR.period_us(500);
  ledG.period_us(500);
  ledB.period_us(500);

  float hue = 0;

  while (1) {
          serial.printf("led value is %u\r\n", (int)led1);

          hue += 0.1;
          wait(1.0/1200);

          float r, g, b;
          hsv_to_rgb_float(hue, 1, 1, &r, &g, &b); // probably can explain a lot more about what is actually going on here in the labs
          r *= r;
          g *= g;
          b *= b;

          r = 1 - r;
          g = 1 - g;
          b = 1 - b;

          ledR = r;
          ledG = g;
          ledB = b;

          serial.printf("led value is %u\r\n", (int)led1);
          if (led1.read()) {
                led1 = 0;
                led2 = 1;
          }
          else {
                led2 = 0;
                led1 = 1;
          }
          if (!btn) {
                led1 = 0;
                led2 = 0;
          }
          serial.printf("led value is %u\r\n", (int)led1);
  }
}
