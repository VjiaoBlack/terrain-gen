#pragma once
/**
 * TrGraphics.hpp
 *
 * Victor Jiao
 *
 * Stores wrappers for SDL2
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include <glm/glm.hpp>

#include "../Perlin.hpp"
#include "../Utils.hpp"
#include "TrMapDataUtils.hpp"

class TrMap;

class TrMapUpdatable {
 public:
  virtual void update(TrMap *map) {};
};

// stores pixels and stuff
template<class T>
class TrMapData : public TrMapUpdatable {
 public:
  T *m_data;

  int m_rows;
  int m_cols;

  TrMapData(int rows, int cols) : m_data(nullptr), m_rows(rows), m_cols(cols) {
    m_data = new T[rows * cols];

    // memset(m_data, 0, rows * cols * sizeof(T));
  }

  virtual ~TrMapData() { delete[] m_data; }

  // defined at bottom of file.
  inline T &at(int r, int c);
  inline T get(int r, int c) const;

  // force float truncating to occur at parameter-passing time.
  T &at(double r, double c) = delete;
  T get(double r, double c) = delete;

  template<class K>
  inline T sample(int r, int c);

  inline T bilerp(double r, double c);

  inline bool isOut(double r, double c);

  void update(TrMap *map) override {};

  void diamondSquare(int s, double level);
  void boxBlur();
  void perlinNoise(unsigned int s, int level, double size, double magnitude);
  std::pair<T, T> getMinMax();
  void set(T t);
};

template<class T>
template<class K>
inline T TrMapData<T>::sample(int r, int c) {
  T sum = {};
  static_assert(K::size % 2 == 1, "TrMapData::sample only supports odd sized kernels.");
  for (int kr = 0; kr < K::size; kr++) {
    for (int kc = 0; kc < K::size; kc++) {
      sum += this->at(r+kr-K::size/2, c+kc-K::size/2) * K::data[kr*K::size+kc];
    }
  }
  return sum;
}

template<class T>
inline T &TrMapData<T>::at(int r, int c) {
  while (r < 0) r += m_rows;
  while (c < 0) c += m_cols;

  while (r >= m_rows) r -= m_rows;
  while (c >= m_cols) c -= m_cols;

  return m_data[r * m_cols + c];
}

template<class T>
inline T TrMapData<T>::bilerp(double r, double c) {
  double r1 = floor(r);
  double r2 = ceil(r);
  double c1 = floor(c);
  double c2 = ceil(c);

  if (r1 == r2) {
    r2++;
  }
  if (c1 == c2) {
    c2++;
  }

  T vtl = this->at((int)floor(r), (int)floor(c));
  T vtr = this->at((int)floor(r), (int)ceil(c));
  T vbl = this->at((int)ceil(r), (int)floor(c));
  T vbr = this->at((int)ceil(r), (int)ceil(c));

  // using formula on https://en.wikipedia.org/wiki/Bilinear_interpolation
  return dot(dvec2(c2 - c, c - c1) * dmat2(vtl, vtr, vbl, vbr),
             dvec2(r2 - r, r - r1));
}

template<class T>
inline T TrMapData<T>::get(int r, int c) const {
  while (r < 0) r += m_rows;
  while (c < 0) c += m_cols;

  while (r >= m_rows) r -= m_rows;
  while (c >= m_cols) c -= m_cols;

  return m_data[r * m_cols + c];
}

template<class T>
inline bool TrMapData<T>::isOut(double r, double c) {
  return r < 0 || c < 0 || r >= m_rows - 1 || c >= m_cols - 1;
}

template<class T>
std::pair<T, T> TrMapData<T>::getMinMax() {
  T min = m_data[0];
  T max = m_data[0];
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      if (this->at(i, j) < min) {
        min = this->at(i, j);
      } else if (this->at(i, j) > max) {
        max = this->at(i, j);
      }
    }
  }
  return make_pair(min, max);
}

template<class T>
void TrMapData<T>::set(T t) {
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      this->at(i, j) = t;
    }
  }
}

// Diamond Square algorithm.
template<class T>
void TrMapData<T>::diamondSquare(int s, double level) {
  if (s < 1) {
    return;
  }

  std::random_device rd;
  std::default_random_engine eg(rd());
  std::uniform_real_distribution<double> dist(-level, level);

  int hs = s / 2;
  for (int i = hs; i < K_MAP_SIZE_Y + hs; i += s) {
    for (int j = hs; j < K_MAP_SIZE_X + hs; j += s) {
      double a = this->at(i - hs, j - hs);
      double b = this->at(i + hs, j - hs);
      double c = this->at(i - hs, j + hs);
      double d = this->at(i + hs, j + hs);
      this->at(i, j) = (a + b + c + d) / 4.0 + dist(eg);
    }
  }

  for (int i = 0; i < K_MAP_SIZE_Y; i += s) {
    for (int j = 0; j < K_MAP_SIZE_X; j += s) {
      double a = this->at(i, j);
      double b = this->at(i + s, j);
      double c = this->at(i + hs, j - hs);
      double d = this->at(i + hs, j + hs);
      double f = this->at(i - hs, j + hs);
      double k = this->at(i, j + s);

      this->at(i + hs, j) = (a + b + c + d) / 4.0 + dist(eg);
      this->at(i, j + hs) = (f + d + a + k) / 4.0 + dist(eg);
    }
  }

  this->diamondSquare(hs, level / 2);
}

// good enough small blur
// even though it's technically incorrect since we immediately place back into
// the grid
template<class T>
void TrMapData<T>::boxBlur() {
  for (int i = 0; i < K_MAP_SIZE_Y; i++) {
    for (int j = 0; j < K_MAP_SIZE_X; j++) {
      T sum = (this->at(i - 1, j - 1) + this->at(i - 1, j) +
          this->at(i - 1, j + 1) + this->at(i, j - 1) +
          this->at(i, j) + this->at(i, j + 1) +
          this->at(i + 1, j - 1) + this->at(i + 1, j) +
          this->at(i + 1, j + 1));

      this->at(i, j) = sum / 9.0;
    }
  }
}

template<class T>
void TrMapData<T>::perlinNoise(unsigned int s, int level, double size,
                               double magnitude) {
  // Create a PerlinNoise object with the reference permutation vector

  PerlinNoise perlin = PerlinNoise();

  if (!level) {
    return;
  }

  // Visit every pixel of the image and assign a color generated with Perlin
  // noise
  // for (unsigned int i = 0; i < s; ++i) {      // y
  //     for (unsigned int j = 0; j < s; ++j) {  // x
  for (int i = 0; i < m_rows; ++i) {    // y
    for (int j = 0; j < m_cols; ++j) {  // x
      double x = (double) j / ((double) (s));
      double y = (double) i / ((double) (s));

      // Typical Perlin noise
      double n = perlin.noise(1.0 + x * size, 1.0 + y * size, 0);

      this->at(i, j) = this->at(i, j) + n * magnitude * 1.2;
    }
  }

  this->perlinNoise(s, level - 1, size * 2.0, magnitude * .4);
}
