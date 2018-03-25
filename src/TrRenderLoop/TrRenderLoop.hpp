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

#include "../TrGame.hpp"
#include "../TrMap/TrMap.hpp"

class TrRenderLoop {
 public:
  TrRenderLoop(){};
  virtual ~TrRenderLoop(){};

  inline virtual TrRenderLoop* update(TrGame* game) { return nullptr; };
  inline virtual void render(TrGame* game){};
};