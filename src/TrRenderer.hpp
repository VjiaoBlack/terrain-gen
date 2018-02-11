
#include <unordered_set>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <random>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

#include "Utils.hpp"
#include "TrGraphics.hpp"

#ifndef _TR_RENDER_HPP_
#define _TR_RENDER_HPP_

class TrRenderer {
public:
    SDL_Window* m_SDLWindow;
    SDL_Renderer* m_SDLRenderer;

    // setup for rendering loop
    SDL_Event m_SDLEvent;
    bool m_quit = false;

    unordered_set<int> m_keysDown;
    unordered_set<char> m_buttonsDown;

    // create texture for map
    SDL_Texture* m_mapTexture;

    TrMap* m_terrain;

    int m_xOff = 0;
    int m_yOff = 0;

    const int c_pixelSize = sz(K_DISPLAY_SIZE) / K_MAP_SIZE;

    int m_speed = 1;

    // initialize random number generator for rain
    std::random_device m_randDevice;
    std::default_random_engine m_randEngine;
    std::uniform_int_distribution<int> m_randDist;

    // initialize framerate counter
    clock_t m_deltaTime = 0;
    unsigned int m_frames = 0;
    double  m_frameRate = 30;


    TrRenderer();
    ~TrRenderer();

    void handleKey(int SDLKey);
    void saveMap();
    void run();

};

#endif