/**
 * TrGraphics.hpp
 *
 * Victor Jiao
 *
 * Stores wrappers for SDL2
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <random>

#include "Perlin.hpp"

#ifndef _TR_GRAPHICS_HPP_
#define _TR_GRAPHICS_HPP_

using namespace std;

// set to 2 (or more) if it's a retina screen, 1 if not.
#define K_RETINA 1
#define sz(x) ((x) * K_RETINA)
#define K_MAP_SIZE 512
#define K_DISPLAY_SIZE 2048

#define K_R_MASK 0x00ff0000
#define K_G_MASK 0x0000ff00
#define K_B_MASK 0x000000ff
#define K_A_MASK 0xff000000

#define K_RGBA_BYTES 32

// edited from
// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float Q_rsqrt(float number);

// I heard that floats are much faster.
struct vec3 {
	float x;
	float y;
	float z;

	vec3(double _x, double _y, double _z)
		: x(_x), y(_y), z(_z) {};

	vec3() : vec3(0.0, 0.0, 0.0) {};

	vec3 operator*(float mult){
		return vec3(x * mult, y * mult, z * mult);
	}

	vec3 operator-(vec3 v) {
		return vec3(x -=v.x, y - v.y, z - v.z);
	}

	inline void normalize() {
		float invsq = Q_rsqrt(x*x + y*y + z*z);
		x *= invsq;
		y *= invsq;
		z *= invsq;
	}

	inline float dot(vec3 v) {
		return x*v.x + y*v.y + z*v.z;
	}

	inline static vec3 cross(vec3 a, vec3 b) {
		vec3 ans;
        ans.x = a.y * b.z - a.z * b.y;
        ans.y = a.z * b.x - a.x * b.z;
        ans.z = a.x * b.y - a.y * b.x;
        return ans;
	}

	inline static vec3 project(vec3 n, vec3 v) {
		// get dist from v to plane n
		// subt dist * n from v
		return v - n * v.dot(n);
	}
};

inline uint32_t lerpColor(uint32_t color1, uint32_t color2, double lerp);
inline uint32_t multiplyColor(uint32_t color, double r, double g, double b);
inline uint32_t shiftColor(uint32_t color, int r, int g, int b);

// stores pixels and stuff
template <class T>
class TrPixels {
public:
	T* m_pixels;

	int m_rows;
	int m_cols;

	TrPixels(int rows, int cols)
		: m_pixels(NULL)
		, m_rows(rows)
		, m_cols(cols) {
		m_pixels = new T[rows * cols];
    	memset(m_pixels, 0, rows * cols * sizeof(T));
	}

	~TrPixels() {
		delete[] m_pixels;
	}

	// defined at bottom of file.
	inline T get(int r, int c);
	inline void set(int r, int c, T p);
	inline T& at(int r, int c);

	void diamondSquare(int s, double level);
	void boxBlur();
	void perlinNoise(int s, int level, double size, double magnitude);
	pair<T, T> getMinMax();
	void set(T t);
};

// map
// is this too much memory? make sure to check
class TrMap {
public:
	// These start off as NULL, and we should allocate as necessary.
	// Or else it might be too much of a waste of memory.
	TrPixels<double>* m_height;
	TrPixels<vec3>* m_normal;
	TrPixels<vec3>* m_wind; 
	TrPixels<double>* m_moisture;
	TrPixels<double>* m_water;
	TrPixels<double>* m_water_temp;
	TrPixels<double>* m_vegetation;

	TrPixels<uint32_t>* m_diffuse;

	int m_rows;
	int m_cols;
	bool m_useMoisture;

	TrMap(int rows, int cols) 
		: m_height(new TrPixels<double>(rows,cols))
		, m_normal(new TrPixels<vec3>(rows,cols))
		, m_wind(new TrPixels<vec3>(rows,cols))
		, m_moisture(new TrPixels<double>(rows,cols))
		, m_water(new TrPixels<double>(rows,cols))
		, m_water_temp(new TrPixels<double>(rows,cols))
		, m_vegetation(new TrPixels<double>(rows,cols))
		, m_diffuse(new TrPixels<uint32_t>(rows,cols))
		, m_rows(rows), m_cols(cols) 
		, m_useMoisture(false) {
		for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                m_normal->at(i,j).z = 1.0f;
                m_wind->at(i,j).y = 1.0f;
            }
        }
	}

	~TrMap() {
		delete m_height;
		delete m_normal;
		delete m_wind;
		delete m_moisture;
		delete m_water;
		delete m_water_temp;
		delete m_vegetation;
		delete m_diffuse;
	}

	void updateColors();
	void updateWater(bool erosion);
	void updateNormals();
	void updateMoisture();
	void updateWind();
};

void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture, int xOff, int yOff, int pixelSize);


#endif 

#include "TrGraphics-impl.hpp"