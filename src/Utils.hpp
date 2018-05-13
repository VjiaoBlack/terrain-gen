
#pragma once

#include <cstdlib>
#include <ctime>
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

#define K_R_MASK 0x00ff0000u
#define K_G_MASK 0x0000ff00u
#define K_B_MASK 0x000000ffu
#define K_A_MASK 0xff000000u

#define K_RGBA_BYTES 32

#define K_FPS 30

using namespace std;
using namespace glm;

void renderTextureWithOffset(SDL_Renderer *renderer, SDL_Texture *texture,
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
  return dvec3((color & 0x00FF0000u) >> 16u, (color & 0x0000FF00u) >> 8u,
               color & 0x000000FFu);
}

inline uint32_t vecToColor(dvec3 color) {
  return 0xFF000000u | ((uint) color.x & 0xFFu) << 16u | ((uint) color.y & 0xFFu) << 8u |
      ((uint) color.z & 0xFFu);
}

inline double clockToMilliseconds(clock_t ticks) {
  return (ticks / (double) CLOCKS_PER_SEC) * 1000.0;
}

inline uint32_t lerpColor(uint32_t color1, uint32_t color2, double lerp) {
  uint32_t cr1 = (color1 & K_R_MASK) >> 16u;
  uint32_t cg1 = (color1 & K_G_MASK) >> 8u;
  uint32_t cb1 = color1 & K_B_MASK;

  uint32_t cr2 = (color2 & K_R_MASK) >> 16u;
  uint32_t cg2 = (color2 & K_G_MASK) >> 8u;
  uint32_t cb2 = color2 & K_B_MASK;

  uint32_t cr = cr1 + (uint32_t) round((cr2 - cr1) * lerp);
  uint32_t cg = cg1 + (uint32_t) round((cg2 - cg1) * lerp);
  uint32_t cb = cb1 + (uint32_t) round((cb2 - cb1) * lerp);

  if (cr > 255) cr = 255;
  if (cg > 255) cg = 255;
  if (cb > 255) cb = 255;

  uint32_t ret = K_A_MASK;
  ret |= cr << 16u;
  ret |= cg << 8u;
  ret |= cb;

  return ret;
}

inline uint32_t multiplyColor(uint32_t color, double r, double g, double b) {
  uint32_t cr = (color & K_R_MASK) >> 16u;
  uint32_t cg = (color & K_G_MASK) >> 8u;
  uint32_t cb = color & K_B_MASK;

  cr = static_cast<uint32_t>(floor(cr * (r > 0 ? r : 0)));
  cg = static_cast<uint32_t>(floor(cg * (g > 0 ? g : 0)));
  cb = static_cast<uint32_t>(floor(cb * (b > 0 ? b : 0)));

  if (cr > 255) cr = 255;
  if (cg > 255) cg = 255;
  if (cb > 255) cb = 255;

  uint32_t ret = K_A_MASK;
  ret |= cr << 16u;
  ret |= cg << 8u;
  ret |= cb;

  return ret;
}

inline uint32_t shiftColor(uint32_t color, int r, int g, int b) {
  int cr = (color & K_R_MASK) >> 16u;
  int cg = (color & K_G_MASK) >> 8u;
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
  ret |= cr << 16u;
  ret |= cg << 8u;
  ret |= cb;

  return ret;
}
