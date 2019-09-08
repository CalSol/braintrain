#include "mbed.h"

#include "ledutils.h"

RGBPwmOut rgbLed(P0_5, P0_6, P0_7);

DigitalOut led1(P1_2);
DigitalOut led2(P1_3);
DigitalOut led3(P0_13);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_11, P0_31);

const uint16_t saturation = 65535;
const uint16_t value      = 32767;

Timer ledPulse;

int main() {
  led1 = 0;
  led2 = 0;
  led3 = 0;
  can.frequency(1000000); // initialize CAN controller to 1 Mbaud
  int prevButton = 1;
  int nextButton;
  ledPulse.start();

  serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");




  while (1) {
    nextButton = btn;
    // read an input message
    CANMessage msg;
    while (can.read(msg)) {
      if (msg.id == 0x43) {
        uint16_t hue = (msg.data[0] << 8) | msg.data[1];
        rgbLed.hsv_uint16(hue, saturation, value);
      } else if (msg.id == 0x42) {
        ledPulse.reset();
        led2 = 1;
      }

      if (ledPulse.read_ms() >= 500) {
        led2 = 0;
      }
    }

    if (ledPulse.read_ms() >= 500) {
      led2 = 0;
    }

    if (nextButton != prevButton && !nextButton) {
      led1 = 1;
      led2 = 1;
      led3 = 1;
      uint16_t blinkLengthMs = 1000;
      uint8_t data[2];
      data[0] = (blinkLengthMs >> 8) & 0xff;
      data[1] = blinkLengthMs & 0xff;
      can.write(CANMessage(0x41, (char*)data, 2));
    }
    prevButton = nextButton;
  }
}
