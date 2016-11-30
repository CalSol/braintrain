#include "mbed.h"

DigitalOut LedR(P0_5);
DigitalOut LedG(P0_6);
DigitalOut LedB(P0_7);

DigitalOut Led1(P0_3);
DigitalOut Led2(P0_9);

int main() {
  LedR = 1;
  LedG = 1;
  LedB = 1;
  Led1 = 0;
  Led2 = 0;

  while (1) {
    LedG = 0;
    LedB = 1;
    wait(0.25);
    LedG = 1;
    LedB = 0;
    wait(0.25);
  }
}
