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
    int threshold[9] = {140, 160, 167, 172, 178, 185, 190, 198, 208};

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
                              sz(1024), sz(512), SDL_WINDOW_SHOWN);
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
            map_height->m_pixels[i * K_MAP_SIZE + j] -= 80;

            if (map_height->m_pixels[i * K_MAP_SIZE + j] < min) {
                min = map_height->m_pixels[i * K_MAP_SIZE + j];
            } else if (map_height->m_pixels[i * K_MAP_SIZE + j] > max) {
                max = map_height->m_pixels[i * K_MAP_SIZE + j];
            }
           
        }
    }
    printf("%d -> %d\n", min, max);

    set_pixels(map_height->m_pixels, map->m_pixels);

    int x = 0;
    int y = 0;

    int pixelSize = sz(512) / K_MAP_SIZE;

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
                    e.motion.x < sz(512)) {
                    int mouseX = e.motion.x / pixelSize;
                    int mouseY = e.motion.y / pixelSize;
                    map_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 0;
                    map->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 0;
                }

                if (buttonsDown.count(SDL_BUTTON_RIGHT) &&
                    e.motion.x < sz(512)) {
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

        

        if (keysDown.count(SDLK_UP))
            y -= pixelSize * speed;
        if (keysDown.count(SDLK_DOWN))
            y += pixelSize * speed; 
        if (keysDown.count(SDLK_LEFT))
            x -= pixelSize * speed;
        if (keysDown.count(SDLK_RIGHT))
            x += pixelSize * speed;

        if (x < 0)
            x += sz(512);
        if (x > sz(512))
            x -= sz(512);
        if (y < 0)
            y += sz(512);
        if (y > sz(512))
            y -= sz(512);

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