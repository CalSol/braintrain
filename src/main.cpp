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
  led1 = 1;
  ledR.write(1);
  ledG.write(1);
  ledB.write(0);
  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");
  serial.printf("led value is %u\r\n", (int)led1);

/*
  while (true) {
    if (btn) {
      led1 = 1;
      wait(.1);
      led1 = 0;
      wait(.1);
    led1 = 1;

    }
    */


    /*
    float hue = 0;
    float r,g,b;

    ledR.period_us(500);
    ledG.period_us(500);
    ledB.period_us(500);

    while (true){
      hue += 0.1;
      wait (1.0/1200);
      hsv_to_rgb_float(hue, 1, 1, &r, &g, &b);
      ledR.write(1 - r*r);
      ledG.write(1 - g*g);
      ledB.write(1 - b*b);
    }

    */

    ledR.period_us(500);
    ledG.period_us(500);
    ledB.period_us(500);


    uint16_t hue = 0;
    uint16_t r,g,b;



    while (true){
      hue += 10;
      hue = hue % 36000;

      wait(1.0/1200);
      hsv_to_rgb_uint16(hue, 65535, 65535, &r, &g, &b);

      ledR.pulsewidth_us((uint32_t)r * 500 / 65535);
      ledG.pulsewidth_us((uint32_t)g * 500 / 65535);
      ledB.pulsewidth_us((uint32_t)b * 500 / 65535);

    }

    
}
