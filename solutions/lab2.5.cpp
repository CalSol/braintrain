#include "mbed.h"

#include "ledutils.h"

DigitalOut led1(LED1);

RawSerial serial(SERIAL_TX, SERIAL_RX, 115200);

RGBPwmOut rgbLed(D9, D11, D12);

DigitalIn btn(D8, PullUp);

CAN can(D10, D2);

int main() {
  // LED Blink State
  Timer ledTimer;  // used to track blink length
  ledTimer.start();

  // Button edge detection
  bool lastButton = true;

  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  while (true) {
    // CAN receive handling
    CANMessage msg;
    while (can.read(msg)) {
      if (msg.id == 0x43) {
        uint16_t hue = (msg.data[0] << 8) | (msg.data[1] << 0);
        rgbLed.hsv_uint16(hue, 65535, 32767);
      } else if (msg.id == 0x42) {
        led1 = 1;
        ledTimer.reset();
      }
    }

    // System actions
    if (ledTimer.read_ms() >= 500) {
      led1 = 0;
    }

    bool thisButton = btn;
    if (thisButton != lastButton && btn == 0) {
      can.write(CANMessage(0x42, (char*)NULL, 0));
    }
    lastButton = thisButton;
  }
}