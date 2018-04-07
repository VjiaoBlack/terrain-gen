
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

using namespace std;

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

template <typename T>
class Vec2;
template <typename T>
class Vec3;
template <typename T>
class Vec4;

template <typename T, int len>
class Vec {
 private:
  friend class Vec2<T>;
  friend class Vec3<T>;
  friend class Vec4<T>;

 public:
  T m_dat[len];
  int m_len;

  Vec() { fill_n(m_dat, len, 0); }

  inline T dot(Vec<T, len> v) {
    T ans = 0;
    for (int i = 0; i < len; i++) {
      ans += m_dat[i] * v[i];
    }

    return ans;
  }

  inline void normalize() {
    T sqsum = 0;
    for (int i = 0; i < len; i++) {
      sqsum += m_dat[i] * m_dat[i];
    }
    sqsum = sqrt(sqsum);
    for (int i = 0; i < len; i++) {
      m_dat[i] /= sqsum;
    }
  }

  inline T& operator[](int i) {
    // TODO: should I do error checking?
    return m_dat[i];
  }

  inline Vec<T, len>& operator*=(const double& mult) {
    for (int i = 0; i < len; i++) {
      m_dat[i] *= mult;
    }

    return *this;
  }
  inline Vec<T, len>& operator-=(const Vec<T, len>& v) {
    for (int i = 0; i < len; i++) {
      m_dat[i] -= v.m_dat[i];
    }
    return *this;
  }

  inline Vec<T, len>& operator+=(const Vec<T, len>& v) {
    for (int i = 0; i < len; i++) {
      m_dat[i] += v.m_dat[i];
    }
    return *this;
  }

  inline Vec<T, len>& operator=(const Vec<T, len>& v) {
    memcpy(m_dat, v.m_dat, sizeof(T) * len);
    return *this;
  }

  inline Vec<T, len> operator*(const double& mult) const {
    Vec<T, len> ans;
    ans = *this;
    ans *= mult;
    return ans;
  }

  inline Vec<T, len> operator-(const Vec<T, len>& v) const {
    Vec<T, len> ans;
    ans = *this;
    ans -= v;
    return ans;
  }

  inline Vec<T, len> operator+(const Vec<T, len>& v) const {
    Vec<T, len> ans;
    ans = *this;
    ans += v;
    return ans;
  }
};

template <typename T>
class Vec2 : public Vec<T, 2> {
 public:
  Vec2() : Vec<T, 2>() { this->m_len = 2; }
  Vec2(Vec<T, 2> v) : Vec<T, 2>() {
    memcpy(this->m_dat, v.m_dat, sizeof(T) * 2);
    this->m_len = 2;
  }

  Vec2(T x, T y) : Vec<T, 2>() {
    this->m_dat[0] = x;
    this->m_dat[1] = y;
    this->m_len = 2;
  }

  T& x = this->m_dat[0];
  T& y = this->m_dat[1];

  // Vec2<T>& operator=(const Vec2<T>& v) { return Vec<T, 2>::operator=(v); }
  // Vec2<T>& operator=(const Vec<T, 2>& v) {
  //   memcpy(this->m_dat, v.m_dat, sizeof(T) * 2);
  //   return *this;
  // }
};

template <typename T>
class Vec3 : public Vec<T, 3> {
 public:
  Vec3()
      : Vec<T, 3>(),
        r(this->m_dat[0]),
        g(this->m_dat[1]),
        b(this->m_dat[2]),
        x(this->m_dat[0]),
        y(this->m_dat[1]),
        z(this->m_dat[2]) {
    this->m_len = 3;
  }
  Vec3(Vec<T, 3> v)
      : Vec<T, 3>(),
        r(this->m_dat[0]),
        g(this->m_dat[1]),
        b(this->m_dat[2]),
        x(this->m_dat[0]),
        y(this->m_dat[1]),
        z(this->m_dat[2]) {
    memcpy(this->m_dat, v.m_dat, sizeof(T) * 3);
    this->r = this->m_dat[0];
    this->g = this->m_dat[1];
    this->b = this->m_dat[2];

    this->x = this->m_dat[0];
    this->y = this->m_dat[1];
    this->z = this->m_dat[2];
    this->m_len = 3;
  }

  Vec3(T x, T y, T z)
      : Vec<T, 3>(),
        r(this->m_dat[0]),
        g(this->m_dat[1]),
        b(this->m_dat[2]),
        x(this->m_dat[0]),
        y(this->m_dat[1]),
        z(this->m_dat[2]) {
    this->m_dat[0] = x;
    this->m_dat[1] = y;
    this->m_dat[2] = z;
    this->m_len = 3;
  }

  T& r;
  T& g;
  T& b;

  T& x;
  T& y;
  T& z;

  inline Vec3<T>& operator=(const Vec3<T>& v) {
    this->m_dat[0] = v.m_dat[0];
    this->m_dat[1] = v.m_dat[1];
    this->m_dat[2] = v.m_dat[2];

    // memcpy(this->m_dat, v.m_dat, sizeof(T) * 3);
    return *this;
  }
  // Vec3<T>& operator=(const Vec<T, 3> v) {

  //   memcpy(this->m_dat, v.m_dat, sizeof(T) * 3);
  //   return *this;
  // }

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

template <typename T>
class Vec4 : public Vec<T, 4> {
 public:
  Vec4() : Vec<T, 4>() { this->m_len = 4; }
  Vec4(Vec<T, 4> v) : Vec<T, 4>() {
    memcpy(this->m_dat, v.m_dat, sizeof(T) * 4);
    this->m_len = 4;
  }

  Vec4(T r, T g, T b, T a) : Vec<T, 4>() {
    this->m_dat[0] = r;
    this->m_dat[1] = g;
    this->m_dat[2] = b;
    this->m_dat[3] = a;
    this->m_len = 4;
  }

  T& r = this->m_dat[0];
  T& g = this->m_dat[1];
  T& b = this->m_dat[2];
  T& a = this->m_dat[3];

  // Vec4<T>& operator=(const Vec4<T>& v) { return Vec<T, 4>::operator=(v); }
  // Vec4<T>& operator=(const Vec<T, 4>& v) {
  //   memcpy(this->m_dat, v.m_dat, sizeof(T) * 4);
  //   return *this;
  // }
};

// class Vec3 {
//  public:
//   double x;
//   double y;
//   double z;

//   Vec3(double _x, double _y, double _z);

//   Vec3();

//   Vec3 operator*(double mult) {
//     return Vec3(x * mult, y * mult, z * mult);
//   }

//   Vec3 operator-(Vec3 v) {
//     return Vec3(x -= v.x, y -= v.y, z -= v.z);
//   }
//   Vec3 operator+(Vec3 v) {
//     return Vec3(x += v.x, y += v.y, z += v.z);
//   }

//   inline void normalize() {
//     double invsq = Q_rsqrt(x * x + y * y + z * z);
//     x *= invsq;
//     y *= invsq;
//     z *= invsq;
//   }

//   inline double dot(Vec3 v) { return x * v.x + y * v.y + z * v.z; }

//   inline static Vec3 cross(Vec3 a, Vec3 b) {
//     Vec3 ans;
//     ans.x = a.y * b.z - a.z * b.y;
//     ans.y = a.z * b.x - a.x * b.z;
//     ans.z = a.x * b.y - a.y * b.x;
//     return ans;
//   }

//   inline static Vec3 project(Vec3 n, Vec3 v) {
//     // get dist from v to plane n
//     // subt dist * n from v
//     return v - n * v.dot(n);
//   }
// };
