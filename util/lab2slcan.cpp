#include "mbed.h"

#include "ledutils.h"
#include "slcan.h"
#include <USBSerial.h>

RGBPwmOut rgbLed(P0_5, P0_6, P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

USBSerial serial(0x1209, 0x0001, 0x0001, false);

CAN can(P0_28, P0_29);
USBSLCANSlave slcan(serial);

const uint16_t DEBOUNCE_TIME_MS = 50;
const uint16_t RGB_LED_UPDATE_MS = 20;  // a respectable 50 Hz

// CAN message transmit helper to echo messages over SLCAN
static bool transmitCANMessage(const CANMessage& msg) {
  if (can.write(msg) == 1) {
    // Echo anything that at least made it to the CAN controller
    // This does not imply that the message was acknowledged.
    if (serial.configured()) {
      slcan.putCANMessage(msg);
    }
    return true;
  }
  return false;
}

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

  // Allow the SLCAN interface to transmit messages
  slcan.setTransmitHandler(&transmitCANMessage);

  // Silently ignore commands to change the mode/bitrate
  // for compatibility with USBtinViewer
  slcan.setIgnoreConfigCommands(true);

  while (true) {
    // CAN receive handling
    CANMessage msg;
    while (can.read(msg)) {
      slcan.putCANMessage(msg);
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
          transmitCANMessage(CANMessage(0x42));
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
      transmitCANMessage(CANMessage(0x43, (char*)data, 3));
    }

    // Process SLCAN commands and report CAN messages
    if (serial.configured()) {
      slcan.update();
    } else {
      serial.connect(false);
    }
  }
}
