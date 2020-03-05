#include "mbed.h"
#include "ledutils.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);

RawSerial serial(P0_8, NC, 115200);

bool whichLED;

int main() 
{
	serial.puts("\r\n\r\n" __FILE__ ", built " __DATE__ " " __TIME__ "\r\n");

	whichLED = 0;
	led1.write(1);
	led2.write(0);

	while (true) 
	{
		if (btn) 
		{
			// ONLY switch if button NOT pressed
			if (whichLED) 
			{
				// Last state was led2 ON so switch
				led1.write(1);
				led2.write(0);
				whichLED = 0;
			}
			else 
			{
				// Last state was led1 ON so switch
				led1.write(0);
				led2.write(1);
				whichLED = 1;
			}
			wait(0.5);
			serial.printf("led1 value is %u\r\n", (int)led1);
			serial.printf("led2 value is %u\r\n", (int)led2);
		}
	}
}

//Simpler Solution
// int main() {
//   while (true) {
//     led1 = 1;
//     led2 = 0;
//     wait(0.5);
//     while(!btn);
//     led1 = 0;
//     led2 = 1;
//     wait(0.5);
//     while(!btn);
//   }
// }
