#include "mbed.h"

#include "ledutils.h"

RGBPwmOut rgbLed(P0_5, P0_6, P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_28, P0_29);

const uint16_t DEBOUNCE_TIME_MS = 50;
const uint16_t RGB_LED_UPDATE_MS = 20;  // a respectable 50 Hz

int main() {
  // LED Blink State
  int32_t blinkLengthMs = 0;
  Timer ledTimer;  // used to track blink length
  ledTimer.start();

  bool lastButton = true;
  Timer buttonDebounceTimer;

  uint16_t hue = 0;
  Timer rgbLedTimer;
  rgbLedTimer.start();

  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  while (true) {
    // CAN receive handling
    CANMessage msg;
    while (can.read(msg)) {
      if (msg.id == 0x42) {
        led2 = 1;
        ledTimer.reset();
        blinkLengthMs = 500;
      } else if (msg.id == 0x41) {
        led2 = 1;
        ledTimer.reset();
        blinkLengthMs = (msg.data[0] << 8) | msg.data[1];
      }
    }

    // System actions
    bool thisButton = btn;
    if (thisButton != lastButton) {
      buttonDebounceTimer.start();
      if (buttonDebounceTimer.read_ms() >= DEBOUNCE_TIME_MS) {
        // Actual edge occurs here
        lastButton = thisButton;

        // Do edge actions
        if (thisButton == false) {
          can.write(CANMessage(0x42));
        }
      }
    }
    if (thisButton == lastButton) {
      // This also gets triggered on a fresh edge.
      buttonDebounceTimer.stop();
      buttonDebounceTimer.reset();
    }

    if (ledTimer.read_ms() >= blinkLengthMs) {
      led2 = 0;
    }

    if (rgbLedTimer.read_ms() >= RGB_LED_UPDATE_MS) {
      rgbLedTimer.reset();

      hue += RGB_LED_UPDATE_MS * 10;
      hue = hue % 36000;  // 360 degree * 100

      rgbLed.hsv_uint16(hue, 65535, 32767);

      // Update CAN with new hue.
      uint8_t data[2];
      data[0] = (hue >> 8) & 0xff;
      data[1] = (hue >> 0) & 0xff;
      can.write(CANMessage(0x43, (char*)data, 3));
    }
  }
}
