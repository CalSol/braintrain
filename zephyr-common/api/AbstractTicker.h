/*
 * AbstractTicker.h
 * Abstraction for hardware tickers.
 *
 *  Created on: Feb 20, 2016
 *      Author: edwardw
 */

#ifndef __ZEPHYR_COMMON_ABSTRACT_TICKER_H__
#define __ZEPHYR_COMMON_ABSTRACT_TICKER_H__

class AbstractTicker {
public:
    AbstractTicker() {};
    virtual ~AbstractTicker() {};

    /**
     * Attach the given function to be called every interval_us microseconds.
     */
    virtual void attach(void (*fn)(void), uint32_t interval_us) = 0;
    /**
     * Detach the ticker.
     */
    virtual void detach() = 0;
};

#endif
