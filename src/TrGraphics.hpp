/**
 * TrGraphics.hpp
 *
 * Victor Jiao
 *
 * Stores wrappers for SDL2
 */

#include <SDL2/SDL.h>

#include <stdlib.h>
#include <time.h>

using namespace std;

// set to 2 (or more) if it's a retina screen, 1 if not.
#define K_RETINA 2
#define sz(x) ((x) * K_RETINA)
#define K_MAP_SIZE 256

class TrPixels {
public:
	uint32_t* m_pixels;
	int m_rows, m_cols;

	TrPixels(int rows, int cols)
			: m_rows(rows)
			, m_cols(cols)
			, m_pixels(NULL) {
		m_pixels = new uint32_t[rows * cols];
    	memset(m_pixels, 0, rows * cols * sizeof(uint32_t));
	}

	~TrPixels() {
		delete[] m_pixels;
	}

	inline uint32_t get(int r, int c) {
		if (r < 0) r += m_rows;
		if (c < 0) c += m_cols;

		if (r >= m_rows) r -= m_rows;
		if (c >= m_cols) c -= m_cols;
		// printf("get %d, %d: %d\n", r, c, m_pixels[r*m_cols + c]);
		return m_pixels[r*m_cols + c];
	}

	inline void set(int r, int c, uint32_t p) {
		if (r < 0) r += m_rows;
		if (c < 0) c += m_cols;

		if (r >= m_rows) r -= m_rows;
		if (c >= m_cols) c -= m_cols;
		// printf("set %d, %d: %d\n", r, c, p);
		m_pixels[r*m_cols + c] = p;
	}
};


void diamondSquare(TrPixels* map, int s, int level);

void boxBlur(TrPixels* map);

void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture, int xOff, int yOff, int pixelSize);
