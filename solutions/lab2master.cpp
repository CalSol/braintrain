#include "mbed.h"

#include "ledutils.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_28, P0_29);

const uint16_t DEBOUNCE_TIME_MS = 50;
const uint16_t RGB_LED_UPDATE_MS = 20;  // a respectable 50 Hz

int main() {
  // Start with RGB LED off
  ledR = 1;
  ledG = 1;
  ledB = 1;

  // LED Blink State
  int32_t blinkLengthMs;
  Timer ledTimer;  // used to track blink length
  ledTimer.start();

  bool lastButton = true;
  Timer buttonDebounceTimer;

  ledR.period_us(500);
  ledG.period_us(500);
  ledB.period_us(500);

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
        blinkLengthMs = 250;
        ledTimer.reset();
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

      uint16_t r, g, b;
      hsv_to_rgb_pwm_uint16(hue, 65535, 65535, &r, &g, &b);

      // Set outputs.
      ledR.pulsewidth_us((uint32_t)r * 500 / 65535);
      ledG.pulsewidth_us((uint32_t)g * 500 / 65535);
      ledB.pulsewidth_us((uint32_t)b * 500 / 65535);

      // Update CAN with new hue.
      uint8_t data[2];
      data[0] = (hue >> 8) & 0xff;
      data[1] = (hue >> 0) & 0xff;
      can.write(CANMessage(0x43, (char*)data, 3));
    }
  }
}
