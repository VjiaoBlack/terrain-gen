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

struct vec3 {
	double x;
	double y;
	double z;
};

// stores pixels and stuff
template <class T>
class TrPixels {
public:
	T* m_pixels;

	int m_rows, m_cols;

	TrPixels(int rows, int cols)
		: m_rows(rows)
		, m_cols(cols)
		, m_pixels(NULL) {
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

	TrPixels<uint32_t>* m_diffuse;

	int m_rows;
	int m_cols;

	TrMap(int rows, int cols) 
		: m_height(new TrPixels<double>(rows,cols))
		, m_normal(new TrPixels<vec3>(rows,cols))
		, m_wind(new TrPixels<vec3>(rows,cols))
		, m_moisture(new TrPixels<double>(rows,cols))
		, m_water(new TrPixels<double>(rows,cols))
		, m_diffuse(new TrPixels<uint32_t>(rows,cols))
		, m_rows(rows), m_cols(cols) {}

	TrMap() : TrMap(0, 0) {}

	~TrMap() {
		delete m_height;
		delete m_normal;
		delete m_wind;
		delete m_moisture;
		delete m_water;
		delete m_diffuse;
	}

	void updateColors();
};

void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture, int xOff, int yOff, int pixelSize);


#endif 

#include "TrGraphics-impl.hpp"