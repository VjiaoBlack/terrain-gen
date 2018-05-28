
#pragma once

#include <cmath>

#include <glm/glm.hpp>

#include "../../src/TrMap/TrMapData.hpp"

// from https://www.keithlantz.net/2011/11/ocean-simulation-part-two-using-the-fast-fourier-transform/

struct vertex_ocean {
  float x, y, z; // vertex
  float nx, ny, nz; // normal
  float a, b, c; // htilde0
  float _a, _b, _c; // htilde0mk conjugate
  float ox, oy, oz; // original position

  vertex_ocean operator-(vertex_ocean v) {
    return (vertex_ocean) {
        x - v.x, y - v.y, z - v.z, // vertex
        nx - v.nx, ny - v.ny, nz - v.nz, // normal
        a - v.a, b - v.b, c - v.c, // htilde0
        _a - v._a, _b - v._b, _c - v._c, // htilde0mk conjugate
        ox - v.ox, oy - v.oy, oz - v.oz
    };
  }

  vertex_ocean operator+(vertex_ocean v) {
    return (vertex_ocean) {
        x + v.x, y + v.y, z + v.z, // vertex
        nx + v.nx, ny + v.ny, nz + v.nz, // normal
        a + v.a, b + v.b, c + v.c, // htilde0
        _a + v._a, _b + v._b, _c + v._c, // htilde0mk conjugate
        ox + v.ox, oy + v.oy, oz + v.oz
    };
  }

  vertex_ocean operator+=(vertex_ocean v) {
    return (*this = *this + v);
  }

};

inline vertex_ocean operator*(vertex_ocean v, float m) {
  return (vertex_ocean) {
      v.x * m, v.y * m, v.z * m, // vertex
      v.nx * m, v.ny * m, v.nz * m, // normal
      v.a * m, v.b * m, v.c * m, // htilde0
      v._a * m, v._b * m, v._c * m, // htilde0mk conjugate
      v.ox * m, v.oy * m, v.oz * m
  };
}

inline vertex_ocean operator*(float m, vertex_ocean v) {
  return (vertex_ocean) {
      v.x * m, v.y * m, v.z * m, // vertex
      v.nx * m, v.ny * m, v.nz * m, // normal
      v.a * m, v.b * m, v.c * m, // htilde0
      v._a * m, v._b * m, v._c * m, // htilde0mk conjugate
      v.ox * m, v.oy * m, v.oz * m
  };
}

class vector3 {
 private:
 protected:
 public:
  float x, y, z;
  vector3();
  vector3(float x, float y, float z);
  float operator*(const vector3 &v);
  vector3 cross(const vector3 &v);
  vector3 operator+(const vector3 &v);
  vector3 operator-(const vector3 &v);
  vector3 operator*(float s);
  vector3 &operator=(const vector3 &v);
  float length();
  vector3 unit();
};

class vector2 {
 private:
 protected:
 public:
  float x, y;
  vector2();
  vector2(float x, float y);
  float operator*(const vector2 &v);
  vector2 operator+(const vector2 &v);
  vector2 operator-(const vector2 &v);
  vector2 operator*(float s);
  vector2 &operator=(const vector2 &v);
  float length();
  vector2 unit();
};

class complex_t {
 private:
 protected:
 public:
  float a, b;
  static unsigned int additions, multiplications;
  complex_t();
  complex_t(float a, float b);
  complex_t conj();
  complex_t operator*(const complex_t &c) const;
  complex_t operator+(const complex_t &c) const;
  complex_t operator-(const complex_t &c) const;
  complex_t operator-() const;
  complex_t operator*(float c) const;
  complex_t &operator=(const complex_t &c);
  static void reset();
};

typedef struct complex_vector_normal {    // structure used with discrete fourier transform
  complex_t h;        // wave height
  vector2 D;        // displacement
  vector3 n;        // normal
} complex_vector_normal;

float uniformRandomVariable();

complex_t gaussianRandomVariable();

class cFFT {
 private:
  unsigned int N, which;
  unsigned int log_2_N;
  float pi2;
  unsigned int *reversed;
  complex_t **T;
  complex_t *c[2];
 protected:
 public:
  explicit cFFT(unsigned int N);
  ~cFFT();
  unsigned int reverse(unsigned int i);
  complex_t t(unsigned int x, unsigned int N);
  void fft(complex_t *input, complex_t *output, int stride, int offset);
};

class cOcean {
 private:
  bool geometry;                // flag to render geometry or surface

  float g;                // gravity constant
  float A;                // phillips spectrum parameter -- affects heights of waves
  vector2 w;                // wind parameter
  complex_t *h_tilde,            // for fast fourier transform
      *h_tilde_slopex, *h_tilde_slopez,
      *h_tilde_dx, *h_tilde_dz;
  cFFT *fft;                // fast fourier transform

  unsigned int *indices;            // indicies for vertex buffer object
  unsigned int indices_count;        // number of indices to render
  // GLuint vbo_vertices, vbo_indices;	// vertex buffer objects

  // GLuint glProgram, glShaderV, glShaderF;	// shaders
  // GLint vertex, normal, texture, light_position, projection, view, model;	// attributes and uniforms

 protected:
 public:
  int N, Nplus1;                // dimension -- N should be a power of 2
  float length;                // length parameter

  // vertex_ocean *vertices;			// vertices for vertex buffer object
  TrMapData<vertex_ocean> *vertices;

  cOcean(int N, float A, vector2 w, float length, bool geometry);
  ~cOcean();
  void release();

  float dispersion(int n_prime, int m_prime);        // deep water
  float phillips(int n_prime, int m_prime);        // phillips spectrum
  complex_t hTilde_0(int n_prime, int m_prime);
  complex_t hTilde(float t, int n_prime, int m_prime);
  complex_vector_normal h_D_and_n(vector2 x, float t);
  void evaluateWaves(float t);
  void evaluateWavesFFT(float t);
  void render(float t, bool use_fft);
};