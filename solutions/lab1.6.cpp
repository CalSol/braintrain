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
  ledR.period_us(500);
  ledG.period_us(500);
  ledB.period_us(500);

  uint16_t hue = 0;

  Timer tim;

  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

  while (true) {
    tim.start();
    tim.reset();
    hue += 10;  // 0.1 degree * 100
    hue = hue % 36000;  // 360 degree * 100

    uint16_t r, g, b;
    hsv_to_rgb_uint16(hue, 65535, 65535, &r, &g, &b);

    // Square intensity to account for human perceived brightness.
    r = (uint32_t)r * r / 65535;
    g = (uint32_t)g * g / 65535;
    b = (uint32_t)b * b / 65535;

    // Invert polarity to account for common-anode LED (emits light when pin is low).
    r = 65535 - r;
    g = 65535 - g;
    b = 65535 - b;

    // Set outputs.
    ledR.pulsewidth_us((uint32_t)r * 500 / 65535);
    ledG.pulsewidth_us((uint32_t)g * 500 / 65535);
    ledB.pulsewidth_us((uint32_t)b * 500 / 65535);

    wait(1.0/1200);
  }
}
