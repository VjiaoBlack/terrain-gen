
#include "Utils.hpp"


void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture, int xOff, int yOff, int pixelSize) {
  SDL_Rect SrcR;
  SDL_Rect DestR;

  SrcR.x = xOff / pixelSize;
  SrcR.y = yOff / pixelSize;
  SrcR.w = K_MAP_SIZE - xOff / pixelSize;
  SrcR.h = K_MAP_SIZE - yOff / pixelSize;

  DestR.x = 0;
  DestR.y = 0;
  DestR.w = sz(K_DISPLAY_SIZE) - xOff;
  DestR.h = sz(K_DISPLAY_SIZE) - yOff;

  SDL_RenderCopy(renderer, texture, &SrcR, &DestR);
}