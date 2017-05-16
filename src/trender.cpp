/**
 * test_driving.cpp
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <unordered_set>


#include "TrGraphics.hpp"


void set_pixels(uint32_t* read_pixels, uint32_t* out_pixels) {
    int threshold[9] = {70, 90, 97, 102, 108, 115, 120, 128, 138};

    uint32_t colors[9] = {0xFF1A2B56,0xFF253C78,0xFF3A5BAA,
                          0xFFEEDDBB,0xFF77BC49,0xFF58A327,
                          0xFF28771F,0xFF210E04,0xFF5B3F31};

    for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            for (int c = 0; c < 9; c++) {
                if (read_pixels[i * K_MAP_SIZE + j] < threshold[c]) {
                    out_pixels[i * K_MAP_SIZE + j] = colors[c];
                    break;
                }

                if (read_pixels[i * K_MAP_SIZE + j] >= threshold[8]) {
                    out_pixels[i * K_MAP_SIZE + j] = 0xFFFFFFFF;
                }
            }
        }
    }
}

int main(int argv, char* argc[]) {
    // Initialize
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create window 
    SDL_Window* window = SDL_CreateWindow("test_driving",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              sz(K_DISPLAY_SIZE), sz(K_DISPLAY_SIZE), SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create renderer for window 
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    
    // setup for rendering loop
	SDL_Event e;
    bool quit = false;

    unordered_set<int> keysDown;
    unordered_set<char> buttonsDown;

    // create texture for map
    SDL_Texture* map_texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, K_MAP_SIZE, K_MAP_SIZE);

    TrPixels* map = new TrPixels(K_MAP_SIZE, K_MAP_SIZE);

    TrPixels* map_height = new TrPixels(K_MAP_SIZE, K_MAP_SIZE);

    // srand
    srand(time(NULL));

    map_height->m_pixels[0] = 255;

    diamondSquare(map_height, K_MAP_SIZE, 48);

    int min = 255;
    int max = 0;
    for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            if (map_height->m_pixels[i * K_MAP_SIZE + j] < min) {
                min = map_height->m_pixels[i * K_MAP_SIZE + j];
            } else if (map_height->m_pixels[i * K_MAP_SIZE + j] > max) {
                max = map_height->m_pixels[i * K_MAP_SIZE + j];
            }
           
        }
    }

    for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            map_height->m_pixels[i * K_MAP_SIZE + j] -= min - 30;
            map_height->m_pixels[i * K_MAP_SIZE + j] *= 2;
            map_height->m_pixels[i * K_MAP_SIZE + j] /= 3;
        }
    }

    min -= min - 30;
    max -= min - 30;

    min *= 2;
    min /= 3;

    max *= 2;
    max /= 3;

    printf("%d -> %d\n", min, max);

    set_pixels(map_height->m_pixels, map->m_pixels);

    int x = 0;
    int y = 0;

    int pixelSize = sz(K_DISPLAY_SIZE) / K_MAP_SIZE;
    // int pixelSize = sz(K_DISPLAY_SIZE) / K_MAP_SIZE;

    int speed = 1;

    while(!quit) {
        SDL_UpdateTexture(map_texture, NULL, map->m_pixels, K_MAP_SIZE * sizeof(uint32_t));
        // Update keysDown and buttonsDown
        while (SDL_PollEvent(&e) != 0) {


            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                keysDown.insert(e.key.keysym.sym);
            } else if (e.type == SDL_KEYUP) {
                keysDown.erase(e.key.keysym.sym);
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                buttonsDown.insert(e.button.button);
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                buttonsDown.erase(e.button.button);
            } else if (e.type == SDL_MOUSEMOTION) {
                if (buttonsDown.count(SDL_BUTTON_LEFT) &&
                    e.motion.x < sz(K_DISPLAY_SIZE)) {
                    int mouseX = e.motion.x / pixelSize;
                    int mouseY = e.motion.y / pixelSize;
                    map_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 0;
                    map->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 0;
                }

                if (buttonsDown.count(SDL_BUTTON_RIGHT) &&
                    e.motion.x < sz(K_DISPLAY_SIZE)) {
                    int mouseX = e.motion.x / pixelSize;
                    int mouseY = e.motion.y / pixelSize;
                    map_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 255;
                    map->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 255;
                }
            }
        }


        if (keysDown.count(SDLK_q)) {
            quit = true;
        }

        if (keysDown.count(SDLK_LSHIFT)) {
            speed = 10;
        } else {
            speed = 1;
        }

        if (keysDown.count(SDLK_u)) {
             for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    map_height->m_pixels[i * K_MAP_SIZE + j] -= speed;
                }
            }

            set_pixels(map_height->m_pixels, map->m_pixels);
            
        }

        if (keysDown.count(SDLK_i)) {
             for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    map_height->m_pixels[i * K_MAP_SIZE + j] += speed;
                }
            }

            set_pixels(map_height->m_pixels, map->m_pixels);
        }

        if (keysDown.count(SDLK_b)) {
            boxBlur(map_height);
            set_pixels(map_height->m_pixels, map->m_pixels);
        }

        // Save the map: color and heightmap
        if (keysDown.count(SDLK_s)) {

            SDL_Surface *surface, *surface2;
            Uint32 rmask, gmask, bmask, amask;

            rmask = 0x00ff0000;
            gmask = 0x0000ff00;
            bmask = 0x000000ff;
            amask = 0xff000000;

            surface = SDL_CreateRGBSurface(0, K_MAP_SIZE, K_MAP_SIZE, 32,
                                   rmask, gmask, bmask, amask);

            SDL_LockSurface(surface);

            memcpy(surface->pixels, map->m_pixels, K_MAP_SIZE * K_MAP_SIZE * 4);


            SDL_UnlockSurface(surface);

            IMG_SavePNG(surface, "color.png");
            SDL_FreeSurface(surface);

            for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    map_height->m_pixels[i * K_MAP_SIZE + j] |= 
                            map_height->m_pixels[i * K_MAP_SIZE + j] << 8;
                    map_height->m_pixels[i * K_MAP_SIZE + j] |= 
                            map_height->m_pixels[i * K_MAP_SIZE + j] << 16;
                    map_height->m_pixels[i * K_MAP_SIZE + j] |= 
                            map_height->m_pixels[i * K_MAP_SIZE + j] << 24;
                    map_height->m_pixels[i * K_MAP_SIZE + j] = 
                            map_height->m_pixels[i * K_MAP_SIZE + j] | 0xFF000000;
                }
            }


            surface2 = SDL_CreateRGBSurface(0, K_DISPLAY_SIZE, K_DISPLAY_SIZE, 32,
                                   rmask, gmask, bmask, amask);

            SDL_LockSurface(surface2);

            memcpy(surface2->pixels, map_height->m_pixels, K_DISPLAY_SIZE * K_DISPLAY_SIZE * 4);

            SDL_UnlockSurface(surface2);

            IMG_SavePNG(surface2, "height.png");
            SDL_FreeSurface(surface2);

        }   

        

        if (keysDown.count(SDLK_UP))
            y -= pixelSize * speed;
        if (keysDown.count(SDLK_DOWN))
            y += pixelSize * speed; 
        if (keysDown.count(SDLK_LEFT))
            x -= pixelSize * speed;
        if (keysDown.count(SDLK_RIGHT))
            x += pixelSize * speed;

        if (x < 0)
            x += sz(K_DISPLAY_SIZE);
        if (x > sz(K_DISPLAY_SIZE))
            x -= sz(K_DISPLAY_SIZE);
        if (y < 0)
            y += sz(K_DISPLAY_SIZE);
        if (y > sz(K_DISPLAY_SIZE))
            y -= sz(K_DISPLAY_SIZE);



        // clear screen
    	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	    SDL_RenderClear(renderer);

            
        renderTextureWithOffset(renderer, map_texture, x, y, pixelSize);

        /** Update screen */
        SDL_RenderPresent(renderer);

        usleep(33 * 1000);
    }

    delete map;
    delete map_height;

    // SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}