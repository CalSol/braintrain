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
  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

  ledR.period_us(500);
  ledG.period_us(500);
  ledB.period_us(500);

  float hue = 0;

  while (true) {
    hue += 0.1;

    float r, g, b;
    hsv_to_rgb_float(hue, 1, 1, &r, &g, &b);

    // Square intensity to account for human perceived brightness.
    r = r * r;
    g = g * g;
    b = b * b;

    // Invert polarity to account for common-anode LED (emits light when pin is low).
    r = 1 - r;
    g = 1 - g;
    b = 1 - b;

    // Set outputs.
    ledR = r;
    ledG = g;
    ledB = b;

    wait(1.0/1200);
  }
}
