#pragma once

/**
 * TrRenderLoop.hpp
 *
 * Includes information about rendering loops
 */

#include <set>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
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

#include "../TrMap/TrMap.hpp"
#include "../TrGame.hpp"

class TrRenderLoop {
public:
	TrRenderLoop() {};
	virtual  ~TrRenderLoop() {};

	virtual void update(const TrGame* game) {};
	virtual void render(const TrGame* game) {};
};