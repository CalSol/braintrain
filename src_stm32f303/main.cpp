#include "mbed.h"

#include "ledutils.h"

//PwmOut ledR(P0_5);
//PwmOut ledG(P0_6);
//PwmOut ledB(P0_7);

DigitalOut led1(LED1);
//DigitalOut led2(P0_9);

//DigitalIn btn(P0_4);

RawSerial serial(SERIAL_TX, NC, 115200);

int main() {
  //ledR.write(1);
  //ledG.write(1);
  //ledB.write(1);
  while (1) {
    led1 = !led1;
    wait(0.1);
    
  }
}
