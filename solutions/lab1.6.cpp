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
  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

  ledR.period_us(500);
  ledG.period_us(500);
  ledB.period_us(500);

  uint16_t hue = 0;

  while (true) {
    hue += 5;  // 0.05 degree * 100
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

    wait_us(1000000 / 2400);
  }
}
