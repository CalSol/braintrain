#include "mbed.h"
#include "rtos.h"

#include "ledutils.h"

RGBPwmOut rgbLed(P0_5, P0_6, P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(P0_28, P0_29);

Mail<CANMessage, 16> canTransmitQueue;

Thread ledThread;
Mail<uint16_t, 1> ledQueue;
void led_thread() {
  while (true) {
    osEvent evt = ledQueue.get();
    if (evt.status == osEventMail) {
      uint16_t waitTime = *(uint16_t*)evt.value.p;
      ledQueue.free((uint16_t*)evt.value.p);
      led2 = 1;
      Thread::wait(waitTime);
      led2 = 0;
    }
  }
}

Thread buttonThread;
void button_thread() {
  bool lastButton = true;
  while (true) {
    bool thisButton = btn;
    if (thisButton != lastButton && btn == 0) {
      CANMessage* msg = canTransmitQueue.alloc(osWaitForever);
      *msg = CANMessage(0x42, NULL, 0);
      canTransmitQueue.put(msg);
    }
    lastButton = thisButton;

    Thread::wait(5);
  }
}

int main() {
  // Initialize CAN controller at 1 Mbaud
  can.frequency(1000000);

  ledThread.start(led_thread);
  buttonThread.start(button_thread);

  while (true) {
    // CAN receive handling
    CANMessage msg;
    while (can.read(msg)) {
      if (msg.id == 0x43) {
        uint16_t hue = (msg.data[0] << 8) | (msg.data[1] << 0);
        rgbLed.hsv_uint16(hue, 65535, 32767);
      } else if (msg.id == 0x42) {
        uint16_t* waitTime = ledQueue.alloc(osWaitForever);
        *waitTime = 500;
        ledQueue.put(waitTime);
      }
    }

    // CAN transmit handling
    osEvent evt = canTransmitQueue.get(0);
    while (evt.status == osEventMail) {
      CANMessage msg = *(CANMessage*)evt.value.p;
      canTransmitQueue.free((CANMessage*)evt.value.p);
      can.write(msg);

      evt = canTransmitQueue.get(0);
    }

    Thread::yield();
  }
}
