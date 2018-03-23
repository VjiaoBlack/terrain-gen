#pragma once

/**
 * TrRenderLoop.hpp
 *
 * Includes information about rendering loops
 */

#include <set>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <iostream>
#include <random>
#include <vector>

#include "../TrMap/TrMap.hpp"

class TrGame;

class TrRenderLoop {
 public:
  TrRenderLoop(){};
  virtual ~TrRenderLoop(){};

  virtual TrRenderLoop* update(TrGame* game) { return nullptr; };
  virtual void render(TrGame* game){};
};