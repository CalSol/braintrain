#include "mbed.h"

#include "ledutils.h"

DigitalOut leds[] = {
  DigitalOut(LED1), // onboard
  DigitalOut(D9, 1),   // R
  DigitalOut(D11, 1),  // G
  DigitalOut(D12, 1)   // B
};

DigitalIn btn(D8, PullUp);

RawSerial serial(SERIAL_TX, SERIAL_RX, 115200);

int main() {
  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

  float d;

  while (true) {
    d = btn ? 0.5 : 0.1;

    for (int i = 0; i < 4; i++) {
      leds[i] = !leds[i];
      wait(d);
      leds[i] = !leds[i];
    }
  }
}
