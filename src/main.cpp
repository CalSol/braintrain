#define CAN_RX P0_29
#define CAN_TX P0_28
#define RX_ID 0x42
#define TX_ID 0x40
#include "mbed.h"

#include "ledutils.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led2(P0_3);
DigitalOut led1(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

CAN can(CAN_RX, CAN_TX);
Ticker canTicker;
Timer blinkTimer;

void sendCAN() {
    uint8_t data[1];
    if (!btn) {
      data[0] = 1;
    } else {
      data[0] = 0;
    }
    can.write(CANMessage(TX_ID, (char*)data, 1));
    led2 = !led2.read();
}

int main() {
  blinkTimer.start();
  canTicker.attach(&sendCAN, 0.25);
  can.frequency(1000000);
  /* BRAIN 1 */
  while (true) {
    CANMessage msg;
    if (can.read(msg)) {
      if (msg.id == RX_ID) {
        led1 = msg.data[0];
      } 
    }
  }
}
