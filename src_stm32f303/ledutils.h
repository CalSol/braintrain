/* LED intensity utilities
 */

#ifndef LEDUTILS_H_
#define LEDUTILS_H_

#include <stdint.h>

#include "mbed.h"

/* Converts H (degrees, [0, 360)), S, V (in [0, 1]), to R, G, B (all in [0, 1])
 */
void hsv_to_rgb_float(float h_deg, float s, float v,
    float *r_out, float *g_out, float *b_out);

/* Converts H (centidegrees, [0, 36000)), S, V (in [0, 65535]),
 * to R, G, B (all in [0, 65535])
 */
void hsv_to_rgb_uint16(uint16_t h_cdeg, uint16_t s, uint16_t v,
    uint16_t *r_out, uint16_t *g_out, uint16_t *b_out);

/* Common-anode RGB LED controlled by PWM, with fixed-point options.
 */
class RGBPwmOut {
public:
  RGBPwmOut(PinName pinR, PinName pinG, PinName pinB):
    ledR(pinR), ledG(pinG), ledB(pinB) {
    ledR.period_us(PWM_PERIOD_US);
    ledG.period_us(PWM_PERIOD_US);
    ledB.period_us(PWM_PERIOD_US);

    // Initialize LEDs to off
    ledR.pulsewidth_us(PWM_PERIOD_US);
    ledG.pulsewidth_us(PWM_PERIOD_US);
    ledB.pulsewidth_us(PWM_PERIOD_US);
  }

  /* Writes a HSV value to the RGB LED, accounting for polarity inversion and
   * human perceived brightness
   * H is in centidegrees, [0, 36000), S, V are in fixed point [0, 65535].
   */
  void hsv_uint16(uint16_t h_cdeg, uint16_t s, uint16_t v);

protected:
  static const uint16_t PWM_PERIOD_US = 500;

  PwmOut ledR;
  PwmOut ledG;
  PwmOut ledB;
};

#endif /* LEDUTILS_H_ */
