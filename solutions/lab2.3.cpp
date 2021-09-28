#include "mbed.h"

#include "ledutils.h"

DigitalOut led1(LED1);

RawSerial serial(SERIAL_TX, SERIAL_RX, 115200);

RGBPwmOut rgbLed(D9, D11, D12);

DigitalIn btn(D8, PullUp);

CAN can(D10, D2); // RX, TX

int main() {
  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);
  can.reset();

  // Button edge detection
  bool lastButton = true;

  while (true) {
    bool thisButton = btn;
    if (thisButton != lastButton && btn == 0) {
      uint16_t blinkLengthMs = 1000;
      uint8_t data[2];
      data[0] = (blinkLengthMs >> 8) & 0xff;
      data[1] = (blinkLengthMs >> 0) & 0xff;
      can.write(CANMessage(0x41, (char*)data, 2));
    }
    lastButton = thisButton;
  }
}
