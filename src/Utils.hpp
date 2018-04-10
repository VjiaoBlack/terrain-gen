
#pragma once

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <glm/glm.hpp>

// set to 2 (or more) if it's a retina screen, 1 if not.
#define K_RETINA 1
#define sz(x) ((x)*K_RETINA)
#define K_MAP_SIZE_X 400
#define K_MAP_SIZE_Y 225
#define K_DISPLAY_SIZE_X 1600
#define K_DISPLAY_SIZE_Y 900

#define K_DISPLAY_SCALE 4

#define K_R_MASK 0x00ff0000
#define K_G_MASK 0x0000ff00
#define K_B_MASK 0x000000ff
#define K_A_MASK 0xff000000

#define K_RGBA_BYTES 32

using namespace std;
using namespace glm;

void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture,
                             int xOff, int yOff, int pixelSize);

// edited from
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
// TODO: fast square root
// TODO: macro
inline double Q_rsqrt(double number) { return 1.0 / sqrt(number); }

inline double lerp5(double omin, double omax, double imin, double xx,
                    double imax) {
  double asdf = 1.0 / (imax - imin);
  return omin * (1.0 - (xx - imin) * asdf) + (xx - imin) * (omax * asdf);
}

inline double lerp3(double omin, double omax, double xx) {
  return omin + xx * (omax - omin);
}

inline dvec3 colorToVec(uint32_t color) {
  //   ocolor.x = (this->at(i, j) & 0x00FF0000) >> 16;
  // ocolor.y = (this->at(i, j) & 0x0000FF00) >> 8;
  // ocolor.z = this->at(i, j) & 0x000000FF;

  return dvec3((color & 0x00FF0000) >> 16, (color & 0x0000FF00) >> 8,
               color & 0x000000FF);
}

inline uint32_t vecToColor(dvec3 color) {
  return 0xFF000000 | ((int)color.x & 0xFF) << 16 | ((int)color.y & 0xFF) << 8 |
         ((int)color.z & 0xFF);
}

inline double clockToMilliseconds(clock_t ticks) {
  return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

inline uint32_t lerpColor(uint32_t color1, uint32_t color2, double lerp) {
  int cr1 = (color1 & K_R_MASK) >> 16;
  int cg1 = (color1 & K_G_MASK) >> 8;
  int cb1 = color1 & K_B_MASK;

  int cr2 = (color2 & K_R_MASK) >> 16;
  int cg2 = (color2 & K_G_MASK) >> 8;
  int cb2 = color2 & K_B_MASK;

  int cr = cr1 + round((cr2 - cr1) * lerp);
  int cg = cg1 + round((cg2 - cg1) * lerp);
  int cb = cb1 + round((cb2 - cb1) * lerp);

  if (cr > 255) cr = 255;
  if (cg > 255) cg = 255;
  if (cb > 255) cb = 255;

  if (cr < 0) cr = 0;
  if (cg < 0) cg = 0;
  if (cb < 0) cb = 0;

  uint32_t ret = K_A_MASK;
  ret |= cr << 16;
  ret |= cg << 8;
  ret |= cb;

  return ret;
}

inline uint32_t multiplyColor(uint32_t color, double r, double g, double b) {
  int cr = (color & K_R_MASK) >> 16;
  int cg = (color & K_G_MASK) >> 8;
  int cb = color & K_B_MASK;

  cr = floor(cr * r);
  cg = floor(cg * g);
  cb = floor(cb * b);

  if (cr > 255) cr = 255;
  if (cg > 255) cg = 255;
  if (cb > 255) cb = 255;

  if (cr < 0) cr = 0;
  if (cg < 0) cg = 0;
  if (cb < 0) cb = 0;

  uint32_t ret = K_A_MASK;
  ret |= cr << 16;
  ret |= cg << 8;
  ret |= cb;

  return ret;
}

inline uint32_t shiftColor(uint32_t color, int r, int g, int b) {
  int cr = (color & K_R_MASK) >> 16;
  int cg = (color & K_G_MASK) >> 8;
  int cb = color & K_B_MASK;

  cr += r;
  cg += g;
  cb += b;

  if (cr > 255) cr = 255;
  if (cg > 255) cg = 255;
  if (cb > 255) cb = 255;

  if (cr < 0) cr = 0;
  if (cg < 0) cg = 0;
  if (cb < 0) cb = 0;

  uint32_t ret = K_A_MASK;
  ret |= cr << 16;
  ret |= cg << 8;
  ret |= cb;

  return ret;
}
