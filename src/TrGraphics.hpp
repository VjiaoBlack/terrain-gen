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
#include <iostream>
#include <time.h>
#include <vector>
#include <random>


#ifndef _TR_GRAPHICS_HPP_
#define _TR_GRAPHICS_HPP_

#include "Utils.hpp"
#include "Perlin.hpp"


using namespace std;


void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture, int xOff, int yOff, int pixelSize);


#endif 

#include "TrGraphics-impl.hpp"