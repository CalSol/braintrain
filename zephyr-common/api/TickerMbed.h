/*
 * TickerMbed.h
 * Tickers implemented with mbed.
 *
 *  Created on: Feb 20, 2016
 *      Author: edwardw
 */

#ifndef __ZEPHYR_COMMON_TICKER_MBED_H__
#define __ZEPHYR_COMMON_TICKER_MBED_H__

#include <mbed.h>

class TickerMbed : public AbstractTicker {
public:
    TickerMbed(Ticker& t) : ticker(t) {
    }

    virtual void attach(void (*fn)(void), uint32_t us) {
        ticker.attach_us(fn, us);
    }

    virtual void detach() {
        ticker.detach();
    }

private:
    Ticker& ticker;
};

#endif
