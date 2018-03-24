
#pragma once

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

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

void renderTextureWithOffset(SDL_Renderer *renderer, SDL_Texture *texture,
                             int xOff, int yOff, int pixelSize);

// edited from
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
// TODO: fast square root
// TODO: macro
inline float Q_rsqrt(float number) { return 1.0f / sqrt(number); }

inline double lerp5(double omin, double omax, double imin, double xx,
                    double imax) {
  double asdf = 1.0 / (imax - imin);
  return omin * (1.0 - (xx - imin) * asdf) + (xx - imin) * (omax * asdf);
}

inline double lerp3(double omin, double omax, double xx) {
  return omin + xx * (omax - omin);
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

// I heard that floats are much faster.
class Vec3 {
 public:
  float x;
  float y;
  float z;

  Vec3(double _x, double _y, double _z) : x(_x), y(_y), z(_z){};

  Vec3() : Vec3(0.0, 0.0, 0.0){};

  Vec3 operator*(float mult) { return Vec3(x * mult, y * mult, z * mult); }

  Vec3 operator-(Vec3 v) { return Vec3(x -= v.x, y - v.y, z - v.z); }

  inline void normalize() {
    float invsq = Q_rsqrt(x * x + y * y + z * z);
    x *= invsq;
    y *= invsq;
    z *= invsq;
  }

  inline float dot(Vec3 v) { return x * v.x + y * v.y + z * v.z; }

  inline static Vec3 cross(Vec3 a, Vec3 b) {
    Vec3 ans;
    ans.x = a.y * b.z - a.z * b.y;
    ans.y = a.z * b.x - a.x * b.z;
    ans.z = a.x * b.y - a.y * b.x;
    return ans;
  }

  inline static Vec3 project(Vec3 n, Vec3 v) {
    // get dist from v to plane n
    // subt dist * n from v
    return v - n * v.dot(n);
  }
};
