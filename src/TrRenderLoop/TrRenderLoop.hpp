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

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <iostream>
#include <random>
#include <vector>

#include "../TrGame.hpp"
#include "../TrMap/TrMap.hpp"

class TrRenderLoop {
 public:
  TrRenderLoop() = default;
  virtual ~TrRenderLoop() = default;

  inline virtual TrRenderLoop *update(TrGame *game) { return this; };
  inline virtual void render(TrGame *game) {};
};