#include <stdlib.h>  // for abs
#include <math.h>  // for fabs, fmod

#include "ledutils.h"

void hsv_to_rgb_float(float h_deg, float s, float v,
    float *r_out, float *g_out, float *b_out) {
  float C = v * s;
  float X = C * (1 - fabs(fmod(h_deg / 60.0, 2) - 1));
  float m = (v - C);
  h_deg = fmod(h_deg, 360);
  if (0 <= h_deg && h_deg < 60) {
    *r_out = C;  *g_out = X;  *b_out = 0;
  } else if (60 <= h_deg && h_deg < 120) {
    *r_out = X;  *g_out = C;  *b_out = 0;
  } else if (120 <= h_deg && h_deg < 180) {
    *r_out = 0;  *g_out = C;  *b_out = X;
  } else if (180 <= h_deg && h_deg < 240) {
    *r_out = 0;  *g_out = X;  *b_out = C;
  } else if (240 <= h_deg && h_deg < 300) {
    *r_out = X;  *g_out = 0;  *b_out = C;
  } else {  // 300 <= H < 360
    *r_out = C;  *g_out = 0;  *b_out = X;
  }
  *r_out = *r_out + m;
  *g_out = *g_out + m;
  *b_out = *b_out + m;
}

void hsv_to_rgb_uint16(uint16_t h_cdeg, uint16_t s, uint16_t v,
    uint16_t *r_out, uint16_t *g_out, uint16_t *b_out) {
  uint16_t C = (uint32_t)v * s / 65535;
  int32_t h_millipct = (((int32_t)h_cdeg * 10 / 60) % 2000) - 1000;  // 1000x
  uint16_t X = C * (1000 - abs(h_millipct)) / 1000;
  uint16_t m = v - C;

  h_cdeg = h_cdeg % 36000;
  if (0 <= h_cdeg && h_cdeg < 6000) {
    *r_out = C;  *g_out = X;  *b_out = 0;
  } else if (6000 <= h_cdeg && h_cdeg < 12000) {
    *r_out = X;  *g_out = C;  *b_out = 0;
  } else if (12000 <= h_cdeg && h_cdeg < 18000) {
    *r_out = 0;  *g_out = C;  *b_out = X;
  } else if (18000 <= h_cdeg && h_cdeg < 24000) {
    *r_out = 0;  *g_out = X;  *b_out = C;
  } else if (24000 <= h_cdeg && h_cdeg < 30000) {
    *r_out = X;  *g_out = 0;  *b_out = C;
  } else {  // 300 <= H < 360
    *r_out = C;  *g_out = 0;  *b_out = X;
  }
  *r_out = *r_out + m;
  *g_out = *g_out + m;
  *b_out = *b_out + m;
}

void RGBPwmOut::hsv_uint16(uint16_t h_cdeg, uint16_t s, uint16_t v) {
  uint16_t C = (uint32_t)v * s / 65535;
  int32_t h_millipct = (((int32_t)h_cdeg * 10 / 60) % 2000) - 1000;  // 1000x
  uint16_t X = C * (1000 - abs(h_millipct)) / 1000;
  uint16_t m = v - C;

  h_cdeg = h_cdeg % 36000;

  uint16_t r, g, b;
  if (0 <= h_cdeg && h_cdeg < 6000) {
    r = C;  g = X;  b = 0;
  } else if (6000 <= h_cdeg && h_cdeg < 12000) {
    r = X;  g = C;  b = 0;
  } else if (12000 <= h_cdeg && h_cdeg < 18000) {
    r = 0;  g = C;  b = X;
  } else if (18000 <= h_cdeg && h_cdeg < 24000) {
    r = 0;  g = X;  b = C;
  } else if (24000 <= h_cdeg && h_cdeg < 30000) {
    r = X;  g = 0;  b = C;
  } else {  // 300 <= H < 360
    r = C;  g = 0;  b = X;
  }

  r = r + m;
  g = g + m;
  b = b + m;

  // Square intensity to account for human perceived brightness.
  r = (uint32_t)r * r / 65535;
  g = (uint32_t)g * g / 65535;
  b = (uint32_t)b * b / 65535;

  // Invert polarity to account for common-anode LED (emits light when pin is low).
  r = 65535 - r;
  g = 65535 - g;
  b = 65535 - b;

  // Write outputs.
  ledR.pulsewidth_us((uint32_t)r * PWM_PERIOD_US / 65535);
  ledG.pulsewidth_us((uint32_t)g * PWM_PERIOD_US / 65535);
  ledB.pulsewidth_us((uint32_t)b * PWM_PERIOD_US / 65535);
}
