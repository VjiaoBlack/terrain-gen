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

    TrMap* ttmap = new TrMap(K_MAP_SIZE, K_MAP_SIZE);

    ttmap->m_height->at(0,0) = 0.5;
    ttmap->m_height->diamondSquare(K_MAP_SIZE, 0.6);
    // ttmap->m_height->perlinNoise(K_MAP_SIZE, 8, 5.0, 0.7);

    double min = 1000.0;
    double max = -1000.0;
    for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            if (ttmap->m_height->at(i,j) < min) {
                min = ttmap->m_height->at(i,j);
            } else if (ttmap->m_height->at(i,j) > max) {
                max = ttmap->m_height->at(i,j);
            }
           
        }
    }

    printf("min: %f\nmax: %f\n", min, max);


    // for (int i = 0; i < K_MAP_SIZE; i++) {
    //     for (int j = 0; j < K_MAP_SIZE; j++) {
    //         ttmap->m_height->at(i,j) = pow(ttmap->m_height->at(i,j),3.0);
    //     }
    // }

    ttmap->updateColors();

    int x = 0;
    int y = 0;

    int pixelSize = sz(K_DISPLAY_SIZE) / K_MAP_SIZE;

    int speed = 1;

    std::random_device rd;
    std::default_random_engine eg(rd());
    std::uniform_int_distribution<int> dist(0, K_MAP_SIZE);


    while(!quit) {
        SDL_UpdateTexture(map_texture, NULL, ttmap->m_diffuse->m_pixels, K_MAP_SIZE * sizeof(uint32_t));
        // SDL_UpdateTexture(map_texture, NULL, map->m_pixels, K_MAP_SIZE * sizeof(uint32_t));
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
                    ttmap->m_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 0;
                }

                if (buttonsDown.count(SDL_BUTTON_RIGHT) &&
                    e.motion.x < sz(K_DISPLAY_SIZE)) {
                    int mouseX = e.motion.x / pixelSize;
                    int mouseY = e.motion.y / pixelSize;
                    ttmap->m_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 255;
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
                    ttmap->m_height->m_pixels[i * K_MAP_SIZE + j] -= (double)speed / 255.0;
                }
            }

            ttmap->updateColors();
        }

        if (keysDown.count(SDLK_i)) {
             for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    ttmap->m_height->m_pixels[i * K_MAP_SIZE + j] += (double)speed / 255.0;
                }
            }

            ttmap->updateColors();
        }

        if (keysDown.count(SDLK_b)) {
            ttmap->m_height->boxBlur();
            ttmap->updateColors();
        }

        // create rain  
        if (keysDown.count(SDLK_r)){
            for (int i = 0; i < K_MAP_SIZE * K_MAP_SIZE / 10; i++) {
                ttmap->m_water->at(dist(eg),dist(eg)) += 0.01;
            }
            ttmap->updateColors();
        }

        // animate water engine
        if (keysDown.count(SDLK_w)) {
            for (int lol = 0; lol< 10; lol++) {

                for (int i = 0; i < K_MAP_SIZE; i++) {
                    for (int j = 0; j < K_MAP_SIZE; j++) {
                        ttmap->m_water->at(i,j) -= 0.00001 * speed * speed * speed;
                        if (ttmap->m_water->at(i,j) < 0.0) {
                            ttmap->m_water->at(i,j) = 0.0;
                        }
                    }
                }

                for (int i = 0; i < K_MAP_SIZE; i++) {
                    for (int j = 0; j < K_MAP_SIZE; j++) {
                        double m = ttmap->m_height->get(i,j) +
                                   ttmap->m_water->get(i,j);
                        int mi = i;
                        int mj = j;
                        double mh = m;

                        for (int ii = i-1; ii <= i+1; ii++) {
                            for (int jj = j-1; jj <= j+1; jj++) {
                                if (ttmap->m_height->get(ii,jj) +
                                    ttmap->m_water->get(ii,jj) < mh) {
                                    mi = ii;
                                    mj = jj;
                                    mh = ttmap->m_height->get(ii,jj) +
                                         ttmap->m_water->get(ii,jj);
                                }
                            }
                        }

                        if (mh < m - 0.0001) {
                            double diff = m-mh;
                            diff *= 0.5;

                            diff *= 1.0 - ttmap->m_water->get(i,j);
                            if (diff > ttmap->m_water->get(i,j)) {
                                diff = ttmap->m_water->get(i,j);
                            }

                            ttmap->m_moisture->at(mi,mj) += diff;
                            ttmap->m_moisture->at(i,j) -= diff;
                        }
                    }
                }

                for (int i = 0; i < K_MAP_SIZE; i++) {
                    for (int j = 0; j < K_MAP_SIZE; j++) {

                        if (keysDown.count(SDLK_e)) {
                            double change = ttmap->m_moisture->at(i,j) > 0 ? 
                                                        0.0 : ttmap->m_moisture->at(i,j);
                            change *= abs(change * 40.0);
                            ttmap->m_height->at(i  ,j  ) += change /  8.0f;
                            ttmap->m_height->at(i+1,j+1) += change / 22.0f;
                            ttmap->m_height->at(i+1,j  ) += change / 16.0f;
                            ttmap->m_height->at(i+1,j-1) += change / 22.0f;
                            ttmap->m_height->at(i  ,j+1) += change / 16.0f;
                            ttmap->m_height->at(i  ,j-1) += change / 16.0f;
                            ttmap->m_height->at(i-1,j+1) += change / 22.0f;
                            ttmap->m_height->at(i-1,j  ) += change / 16.0f;
                            ttmap->m_height->at(i-1,j-1) += change / 22.0f;
                        }
                        ttmap->m_water->at(i,j) += ttmap->m_moisture->at(i,j);
                        if (ttmap->m_water->at(i,j) < 0.0) {
                            ttmap->m_water->at(i,j) = 0.0;
                        } 
                        if (ttmap->m_water->at(i,j) > 1.0) {
                            ttmap->m_water->at(i,j) = 1.0;
                        }

                        ttmap->m_moisture->at(i,j) = 0.0;
                    }
                }
            }

            ttmap->updateColors();
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

            memcpy(surface->pixels, ttmap->m_diffuse->m_pixels, K_MAP_SIZE * K_MAP_SIZE * 4);


            SDL_UnlockSurface(surface);

            IMG_SavePNG(surface, "color.png");
            SDL_FreeSurface(surface);

            TrPixels<uint32_t> map_height(K_MAP_SIZE, K_MAP_SIZE);


            for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    map_height.at(i,j) = floor(ttmap->m_height->at(i,j) * 255);
                }
            }


            for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    map_height.m_pixels[i * K_MAP_SIZE + j] |= 
                            map_height.m_pixels[i * K_MAP_SIZE + j] << 8;
                    map_height.m_pixels[i * K_MAP_SIZE + j] |= 
                            map_height.m_pixels[i * K_MAP_SIZE + j] << 16;
                    map_height.m_pixels[i * K_MAP_SIZE + j] |= 
                            map_height.m_pixels[i * K_MAP_SIZE + j] << 24;
                    map_height.m_pixels[i * K_MAP_SIZE + j] = 
                            map_height.m_pixels[i * K_MAP_SIZE + j] | 0xFF000000;
                }
            }


            surface2 = SDL_CreateRGBSurface(0, K_DISPLAY_SIZE, K_DISPLAY_SIZE, 32,
                                   rmask, gmask, bmask, amask);

            SDL_LockSurface(surface2);

            memcpy(surface2->pixels, map_height.m_pixels, K_DISPLAY_SIZE * K_DISPLAY_SIZE * 4);

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

    delete ttmap;

    // SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}