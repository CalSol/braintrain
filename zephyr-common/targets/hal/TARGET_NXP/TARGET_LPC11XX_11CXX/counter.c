/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stddef.h>
#include "counter_api.h"
#include "PeripheralNames.h"
#include <bitfields.h>
#include "pinmap.h"

#define COUNTER_TIMER          ((LPC_TMR_TypeDef *)LPC_CT32B0_BASE)
#define COUNTER_TIMER_IRQn     TIMER_32_0_IRQn

static int counter_inited = 0;

static const PinMap PinMap_Counter[] = {
    /* CT32B0 */
    {P1_5 , PWM_5, 0x02},   /* CR0 */

    {NC   , NC    ,0x00}
};

void counter_init(counter_event_type event, int prescale) {
    if (!counter_inited) {
        // Enable the clock to timer 0
        LPC_SYSCON->SYSAHBCLKCTRL |= SYSAHBCLKCTRL_CT32B0; // Clock TIMER_0

        // Reset and disable the timer
        COUNTER_TIMER->TCR = TMR32B0TCR_CRST;

        // Disable the match interrupts and outputs
        COUNTER_TIMER->MCR = 0;
        COUNTER_TIMER->EMR = 0;
        // Disable the capture interrupts
        COUNTER_TIMER->CCR = 0;
        // Disable PWM
        COUNTER_TIMER->PWMC = 0;

        // No interrupts for counters
        NVIC_DisableIRQ(COUNTER_TIMER_IRQn);

        pinmap_pinout(P1_5, PinMap_Counter);

        counter_inited = 1;
    }

    counter_mode(event, prescale);
}

void counter_mode(counter_event_type event, int prescale) {
    // Disable the counter
    COUNTER_TIMER->TCR = 0;

    // Set the new count mode
    COUNTER_TIMER->CTCR = BFN_PREP(event, TMR32B0CTCR_CTM) | BFN_PREP(0, TMR32B0CTCR_CIS);

    // Set the new prescaler
    if (prescale > 0) {
        COUNTER_TIMER->PR = prescale - 1;
    } else {
        COUNTER_TIMER->PR = 0;
    }

    // Re-enable the counter
    COUNTER_TIMER->TCR = TMR32B0TCR_CEN;
}
uint32_t counter_read() {
	return COUNTER_TIMER->TC;
}
