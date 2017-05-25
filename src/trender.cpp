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
#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <unordered_set>


#include "TrGraphics.hpp"


inline double clockToMilliseconds(clock_t ticks){
    return (ticks/(double)CLOCKS_PER_SEC)*1000.0;
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

    TrMap* terrain = new TrMap(K_MAP_SIZE, K_MAP_SIZE);

    // terrain->m_height->at(0,0) = 0.5;
    // terrain->m_height->diamondSquare(K_MAP_SIZE, 0.8);
    terrain->m_height->perlinNoise(K_MAP_SIZE, 8, 2.0, 0.7);

    pair<double, double> minMax = terrain->m_height->getMinMax();
    cout << "min: " << minMax.first << endl;
    cout << "max: " << minMax.second << endl;

    for (int i = 0; i < terrain->m_rows; i++) {
        for (int j = 0; j < terrain->m_cols; j++) {
            terrain->m_height->at(i,j) = (terrain->m_height->at(i,j) - minMax.first) * 
                                         (1.0 / (minMax.second - minMax.first));
            terrain->m_height->at(i,j) = pow(terrain->m_height->at(i,j), 1);
        }
    }


    terrain->updateColors();

    int xOff = 0;
    int yOff = 0;

    int pixelSize = sz(K_DISPLAY_SIZE) / K_MAP_SIZE;

    int speed = 1;

    // initialize random number generator for rain
    std::random_device rd;
    std::default_random_engine eg(rd());
    std::uniform_int_distribution<int> dist(0, K_MAP_SIZE);

    // initialize framerate counter
    clock_t deltaTime = 0;
    unsigned int frames = 0;
    double  frameRate = 30;
    double  averageFrameTimeMilliseconds = 33.333;

    while(!quit) {
        clock_t beginFrame = clock();

        SDL_UpdateTexture(map_texture, NULL, terrain->m_diffuse->m_pixels, K_MAP_SIZE * sizeof(uint32_t));

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
                    terrain->m_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 0;
                }

                if (buttonsDown.count(SDL_BUTTON_RIGHT) &&
                    e.motion.x < sz(K_DISPLAY_SIZE)) {
                    int mouseX = e.motion.x / pixelSize;
                    int mouseY = e.motion.y / pixelSize;
                    terrain->m_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 255;
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
                    terrain->m_height->m_pixels[i * K_MAP_SIZE + j] -= (double)speed / 255.0;
                }
            }
            terrain->updateColors();
        }

        if (keysDown.count(SDLK_i)) {
             for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    terrain->m_height->m_pixels[i * K_MAP_SIZE + j] += (double)speed / 255.0;
                }
            }
            terrain->updateColors();
        }

        if (keysDown.count(SDLK_b)) {
            for (int i = 0; i < speed; i++) {
                terrain->m_height->boxBlur();
            }
            terrain->updateColors();
        }

        // create rain  
        if (keysDown.count(SDLK_r)){
            for (int i = 0; i < speed * K_MAP_SIZE * K_MAP_SIZE / 1000; i++) {
                terrain->m_water->at(dist(eg),dist(eg)) += 0.01;
            }
            terrain->updateColors();
        }

        if (keysDown.count(SDLK_d)) {
            terrain->m_water->set(0.0);
            terrain->updateColors();
        }

        // animate water engine
        if (keysDown.count(SDLK_w)) {
            for (int i = 0; i < speed; i++) {
                terrain->updateWater(keysDown.count(SDLK_e));
            }
            terrain->updateColors();
        }

        // reclaculate normals
        if (keysDown.count(SDLK_n)) {
            terrain->updateNormals();
            terrain->updateColors();
        }

        // update water!! omg 
        if (keysDown.count(SDLK_m)) {
            for (int i = 0; i < speed; i++) {
                terrain->updateMoisture();
                terrain->updateColors();
            }
        }

        // toggle moisture / not
        if (keysDown.count(SDLK_k)) {
            // or else it would blink tooo fastttt
            usleep(100000);
            terrain->m_useMoisture = !terrain->m_useMoisture;
            terrain->updateColors();
        }

        if (keysDown.count(SDLK_j)) {
            terrain->updateWind();
        }

        // Save the map: color and heightmap
        if (keysDown.count(SDLK_s)) {

            SDL_Surface *surface, *surface2;

            surface = SDL_CreateRGBSurface(0, K_MAP_SIZE, K_MAP_SIZE, K_RGBA_BYTES,
                                           K_R_MASK, K_G_MASK, K_B_MASK, K_A_MASK);

            SDL_LockSurface(surface);

            memcpy(surface->pixels, terrain->m_diffuse->m_pixels, K_MAP_SIZE * K_MAP_SIZE * 4);


            SDL_UnlockSurface(surface);

            IMG_SavePNG(surface, "color.png");
            SDL_FreeSurface(surface);

            TrPixels<uint32_t> map_height(K_MAP_SIZE, K_MAP_SIZE);


            for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    map_height.at(i,j) = floor(terrain->m_height->at(i,j) * 255);
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


            surface2 = SDL_CreateRGBSurface(0, K_MAP_SIZE, K_MAP_SIZE, K_RGBA_BYTES,
                                            K_R_MASK, K_G_MASK, K_B_MASK, K_A_MASK);

            SDL_LockSurface(surface2);
            memcpy(surface2->pixels, map_height.m_pixels, K_MAP_SIZE * K_MAP_SIZE * sizeof(uint32_t));
            SDL_UnlockSurface(surface2);

            IMG_SavePNG(surface2, "height.png");

            SDL_FreeSurface(surface2);
        }   

        

        if (keysDown.count(SDLK_UP))
            yOff -= pixelSize * speed;
        if (keysDown.count(SDLK_DOWN))
            yOff += pixelSize * speed; 
        if (keysDown.count(SDLK_LEFT))
            xOff -= pixelSize * speed;
        if (keysDown.count(SDLK_RIGHT))
            xOff += pixelSize * speed;

        if (xOff < 0)
            xOff += sz(K_DISPLAY_SIZE);
        if (xOff > sz(K_DISPLAY_SIZE))
            xOff -= sz(K_DISPLAY_SIZE);
        if (yOff < 0)
            yOff += sz(K_DISPLAY_SIZE);
        if (yOff > sz(K_DISPLAY_SIZE))
            yOff -= sz(K_DISPLAY_SIZE);

        // clear screen
    	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	    SDL_RenderClear(renderer);
            
        renderTextureWithOffset(renderer, map_texture, xOff, yOff, pixelSize);

        // update screen
        SDL_RenderPresent(renderer);

        clock_t endFrame = clock();

        deltaTime += endFrame - beginFrame;
        frames ++;

        // display FPS
        if (clockToMilliseconds(deltaTime)>1000.0){ //every second
            frameRate = (double)frames; //more stable
            frames = 0;
            deltaTime -= CLOCKS_PER_SEC;
            averageFrameTimeMilliseconds  = 1000.0/(frameRate==0?0.001:frameRate);

            std::cout <<"fps: " << frameRate << "        \r" ;
            std::flush(std::cout);
        }
    }

    delete terrain;

    // SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}