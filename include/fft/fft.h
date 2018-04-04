#pragma once 

#include <math.h>
#include "complex.h"


// from https://www.keithlantz.net/2011/11/ocean-simulation-part-two-using-the-fast-fourier-transform/

struct vertex_ocean {
	float   x,   y,   z; // vertex
	float  nx,  ny,  nz; // normal
	float   a,   b,   c; // htilde0
	float  _a,  _b,  _c; // htilde0mk conjugate
	float  ox,  oy,  oz; // original position
};




class vector3 {
  private:
  protected:
  public:
    float x, y, z;
    vector3();
    vector3(float x, float y, float z);
    float operator*(const vector3& v);
    vector3 cross(const vector3& v);
    vector3 operator+(const vector3& v);
    vector3 operator-(const vector3& v);
    vector3 operator*(const float s);
    vector3& operator=(const vector3& v);
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
    float operator*(const vector2& v);
    vector2 operator+(const vector2& v);
    vector2 operator-(const vector2& v);
    vector2 operator*(const float s);
    vector2& operator=(const vector2& v);
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
    complex_t operator*(const complex_t& c) const;
    complex_t operator+(const complex_t& c) const;
    complex_t operator-(const complex_t& c) const;
    complex_t operator-() const;
    complex_t operator*(const float c) const;
    complex_t& operator=(const complex_t& c);
    static void reset();
};


typedef struct complex_vector_normal {	// structure used with discrete fourier transform
	complex_t h;		// wave height
	vector2 D;		// displacement
	vector3 n;		// normal
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
	cFFT(unsigned int N);
	~cFFT();
	unsigned int reverse(unsigned int i);
	complex_t t(unsigned int x, unsigned int N);
	void fft(complex_t* input, complex_t* output, int stride, int offset);
};


class cOcean {
  private:
	bool geometry;				// flag to render geometry or surface

	float g;				// gravity constant
	float A;				// phillips spectrum parameter -- affects heights of waves
	vector2 w;				// wind parameter
	complex_t *h_tilde,			// for fast fourier transform
		*h_tilde_slopex, *h_tilde_slopez,
		*h_tilde_dx, *h_tilde_dz;
	cFFT *fft;				// fast fourier transform

	unsigned int *indices;			// indicies for vertex buffer object
	unsigned int indices_count;		// number of indices to render
	// GLuint vbo_vertices, vbo_indices;	// vertex buffer objects

	// GLuint glProgram, glShaderV, glShaderF;	// shaders
	// GLint vertex, normal, texture, light_position, projection, view, model;	// attributes and uniforms

  protected:
  public:
	int N, Nplus1;				// dimension -- N should be a power of 2
	float length;				// length parameter

	vertex_ocean *vertices;			// vertices for vertex buffer object

	cOcean(const int N, const float A, const vector2 w, const float length, bool geometry);
	~cOcean();
	void release();

	float dispersion(int n_prime, int m_prime);		// deep water
	float phillips(int n_prime, int m_prime);		// phillips spectrum
	complex_t hTilde_0(int n_prime, int m_prime);
	complex_t hTilde(float t, int n_prime, int m_prime);
	complex_vector_normal h_D_and_n(vector2 x, float t);
	void evaluateWaves(float t);
	void evaluateWavesFFT(float t);
	void render(float t, bool use_fft);
};