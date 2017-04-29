/**
 * TrGraphics
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include "TrGraphics.hpp"




// Diamond Square algorithm.
void diamondSquare(TrPixels* map, int s, int level) {
	if (s < 1) {
		return;
	}
	// printf("Stage: %d\n", s);

	// printf("Diamond\n");
	uint32_t* p = map->m_pixels;
	for (int i = s; i <= K_MAP_SIZE; i += s) {
		for (int j = s; j <= K_MAP_SIZE; j += s) {
			int a = map->get(i-s, j-s);
			int b = map->get(i  , j-s);
			int c = map->get(i-s, j  );
			int d = map->get(i  , j  );
			int e = (a + b + c + d) / 4 + rand() % level - level / 2;
			map->set(i-s/2, j-s/2, e);

		}
		// printf("--\n");
	}

	// printf("Square\n");

	for (int i = s; i <= K_MAP_SIZE; i += s) {
		for (int j = s; j <= K_MAP_SIZE; j += s) {
			int a = map->get(i-s, j-s);
			int b = map->get(i  , j-s);
			int c = map->get(i-s, j  );
			int d = map->get(i  , j  );
			int e = map->get(i-s/2, j-s/2);

			int f = (a + c + e + map->get(i-3*s/2, j-s/2)) / 4 + rand() % level - level / 2;
			int g = (a + b + e + map->get(i-s/2, j-3*s/2)) / 4 + rand() % level - level / 2;
			
			map->set(i-s, j-s/2, f);
			map->set(i-s/2, j-s, g);
		}
		// printf("--\n");
	}

	diamondSquare(map, s / 2, level);
}


void boxBlur(TrPixels* map) {
	for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            int sum = (map->get(i-1, j-1) +
                       map->get(i-1, j  ) +
                       map->get(i-1, j+1) +
                       map->get(i  , j-1) +
                       map->get(i  , j  ) +
                       map->get(i  , j+1) +
                       map->get(i+1, j-1) +
                       map->get(i+1, j  ) +
                       map->get(i+1, j+1));

            map->m_pixels[i * K_MAP_SIZE + j] = sum / 9;

            // without this, all values will decrease
            if (sum % 9 > 4) {
                map->m_pixels[i * K_MAP_SIZE + j]++;
            }
        }
    }  
}


void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture, int xOff, int yOff, int pixelSize) {
	SDL_Rect SrcR;
    SDL_Rect DestR;

    SrcR.x = xOff / pixelSize;
    SrcR.y = yOff / pixelSize;
    SrcR.w = K_MAP_SIZE - xOff / pixelSize;
    SrcR.h = K_MAP_SIZE - yOff / pixelSize;

    DestR.x = 0;
    DestR.y = 0;
    DestR.w = sz(512) - xOff;
    DestR.h = sz(512) - yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);


    SrcR.x = 0;
    SrcR.y = 0;
    SrcR.w = xOff / pixelSize;
    SrcR.h = yOff / pixelSize;

    DestR.x = sz(512) - xOff;
    DestR.y = sz(512) - yOff;
    DestR.w = xOff;
    DestR.h = yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);


    SrcR.x = 0;
    SrcR.y = yOff / pixelSize;
    SrcR.w = xOff / pixelSize;
    SrcR.h = K_MAP_SIZE;

    DestR.x = sz(512) - xOff;
    DestR.y = 0;
    DestR.w = xOff;
    DestR.h = sz(512) - yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);


    SrcR.x = xOff / pixelSize;
    SrcR.y = 0;
    SrcR.w = K_MAP_SIZE;
    SrcR.h = yOff / pixelSize;

    DestR.x = 0;
    DestR.y = sz(512) - yOff;
    DestR.w = sz(512) - xOff;
    DestR.h = yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);

}
