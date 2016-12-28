# lab1: Embedded Introduction

## Objectives
Target audience
- Anyone with no practical experience with Arduino-level embedded systems or with the mbed API.
  - All electrical team members, whether focusing on software or hardware, should attend. 
  - Anyone in general (even the mech people!) that is interested is also welcome to attend.

Assumptions:
- You know are familiar with imperative programming languages (like C) and object-oriented style (like C++ / Java), and can at least stumble through C++ syntax.
- You know the very basics of electronics engineering: common components (LED, resistor, and switch) and how to use them as well as reading schematics.

Objectives
- Play with inputs (button) and outputs (LEDs) controlled by software running on an embedded microcontroller.

## Introduction
In the context of CalSol, an embedded system refers to a microcontroller-based system, which runs code (firmware) that interacts with (through inputs such as sensors or buttons, and outputs such as actuators or LEDs) the physical world.

A microcontroller is basically a full computer system on a chip, including a CPU, program memory, data memory, and IO. You'll be working with the BRAINv3.3 (which you've already soldered), containing a LPC1549 that has a 72MHz Cortex-M3 processor, 256KiB of FLASH program memory, 4KiB of data memory, and 30 IO pins. We've also standardized on using the [mbed](https://www.mbed.com/en/) library to present a nicer API to control the underlying hardware.

This lab will walk through some simple examples to get started.

## Lab 1.1: Getting started
> Do these before the lab:
> 0. Solder your own BRAINv3.3 (soldering training).
> 0. Set up [the build system](../Zephyr-FW#setup). _In the future, we may add instructions for using the mbed online compiler for those whose primary focus isn't firmware development._

0. If doing this lab during one of the scheduled training sessions, consider pairing up.
0. Clone this repository. You may also set up this repository under Eclipse (similarly to [Zephyr-FW](../Zephyr-FW#project-configuration)), or use command-line scons and openocd.
0. Sanity check: build the code, either by running `scons` at the repository root, or in Eclipse. 

## Lab 1.2: "Hello, world"
While the typical programming "Hello, world" is to print text on a screen, we don't have a screen on our BRAINs. Instead, we will do the typical embedded "hello, world", which is to blink a LED.

Start by taking a look at [src/main.cpp]. At the top, you will see mbed object declarations that assign certain functionality (like `DigitalOut`) to pins (like `P0_3`):

```c++
#include "mbed.h"

PwmOut ledR(P0_5);
PwmOut ledG(P0_6);
PwmOut ledB(P0_7);

DigitalOut led1(P0_3);
DigitalOut led2(P0_9);

DigitalIn btn(P0_4);
  
RawSerial serial(P0_8, NC);
``` 

**Objective**: for this first part will be to alternate the left LED (`led1`, on `P0_3`) and the right LED (`led2`, on `P0_9`) on and off, at once full cycle (left on, then right on) per second.

> LEDs are electronic devices that emit light when current flows through them from anode (A) to cathode (K).
> The circuit on the BRAINv3.3 for the left and right LEDs is: 
> (diagram here) 
> The LED is turned on (emits light) when the pin voltage is high.

mbed's [DigitalOut](https://developer.mbed.org/handbook/DigitalOut) provides a way to control an IO pin controlled as a digital output - the pin can be set low (0v in this case) or high (3.3v in this case). Note that from the [example and API docs](https://developer.mbed.org/handbook/DigitalOut), this can be accomplished by setting the object to 0 or 1. For example,

```c++
led1 = 1;
```

will turn on the left LED, while

```c++
led1 = 0;
```

will turn off the left LED.

Feel free to try playing with the LEDs by writing some code and deploying it.

> In C and C++, programs start inside the `main()` function. We've given you a empty `main()` function here:
>
> ```c++
> int main() {
>   /* YOUR INITIALIZATION HERE */
>
>   while (true) {
>     /* YOUR CODE HERE */
>   }
> }
> ```
>
> This has been structured as typical embedded code: an (optional) initialization section, followed by code repeating forever in a main loop.  

mbed also has basic timing functions, like [Wait](https://developer.mbed.org/handbook/Wait), that waits for a specified number of seconds regardless (given some assumptions) of the underlying hardware. This may be useful for getting the (approximate) 1Hz blink rate. 

Done? Compare against [the solution here](solutions/lab1.2.cpp).

## 1.3: Now with _inputs_!
A system that only produces outputs isn't much fun. Let's do something with the user button (left one).

**Objective**: Pause the blinking as long as the button is pressed.

Like DigitalOut, mbed also has a [DigitalIn](https://developer.mbed.org/handbook/DigitalIn) that allows it to read the logic level on a pin, also as either low (0v to some threshold, in this case) or high (some threshold to 3.3v, in this case). Since `DigitalIn` provides an implicit conversion to an `int`, it can be used anywhere that expects an `int`, for example as a conditional:

```c++
if (!btn) {
  /* do something if button is pressed */
}

```  

> Switches are electronic devices that connect (or disconnect, in some cases) a circuit when pressed.
> The circuit on the BRAINv3.3 for the user button is:
> (diagram here)
> Commonly, user buttons are designed like this: a resistor pulls up the pin high when the switch is not pressed, and the switch shorts the pin low when the button is pressed. This also explains the inversion (`!btn`) in the example above to detect a pressed (low) state.

Done? Compare against [one possible solution here](solutions/lab1.3.cpp).

You may realize that while the high-level objective (pause blinking) might appear clear, there are many different implementations that could result in subtly different behaviors. For example, it's unspecified whether pressing the button should only stop toggling the LED (as in the example solution) or if it should also pause the counter (for example, if you press the button 0.25s after the last toggle, it shouldn't toggle for 0.25s after you release the button). For a toy example and at these timescales for human reaction, it's inconsequential, but that won't be true for every system you work on...  

## Lab 1.4: "Hello, world", but for reals


## Lab 1.5: "Hello, world", _in color_


## Extra for Experts Lab 1.6: Embedded constraints


## To infinity and beyond
 