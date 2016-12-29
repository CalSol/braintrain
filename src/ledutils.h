/* LED intensity utilities
 */

#ifndef LEDUTILS_H_
#define LEDUTILS_H_

#include <stdint.h>

/* Converts H (degrees, [0, 360)), S, V (in [0, 1]), to R, G, B (all in [0, 1])
 */
void hsv_to_rgb_float(float h_deg, float s, float v,
    float *r_out, float *g_out, float *b_out);

/* Converts H (centidegrees, [0, 36000)), S, V (in [0, 65535]),
 * to R, G, B (all in [0, 65535])
 */
void hsv_to_rgb_uint16(uint16_t h_cdeg, uint16_t s, uint16_t v,
    uint16_t *r_out, uint16_t *g_out, uint16_t *b_out);

#endif /* LEDUTILS_H_ */
