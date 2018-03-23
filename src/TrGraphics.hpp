#pragma once
/**
 * TrGraphics.hpp
 *
 * Victor Jiao
 *
 * Stores wrappers for SDL2
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <random>
#include <vector>

#include "Perlin.hpp"
#include "Utils.hpp"

using namespace std;

void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture,
                             int xOff, int yOff, int pixelSize);

#include "TrGraphics-impl.hpp"