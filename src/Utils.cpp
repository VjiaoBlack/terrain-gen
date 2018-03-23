
#include "Utils.hpp"

// TODO: is this even useful anymore? 
void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture,
                             int xOff, int yOff, int pixelSize) {
  SDL_Rect SrcR;
  SDL_Rect DestR;

  // SrcR.x = xOff / pixelSize;
  // SrcR.y = yOff / pixelSize;
  // SrcR.w = K_MAP_SIZE_X - xOff / pixelSize;
  // SrcR.h = K_MAP_SIZE_Y - yOff / pixelSize;

  // DestR.x = 0;
  // DestR.y = 0;
  // DestR.w = sz(K_DISPLAY_SIZE_X) - xOff;
  // DestR.h = sz(K_DISPLAY_SIZE_Y) - yOff;

  SrcR.x = 0;
  SrcR.y = 0;
  SrcR.w = K_MAP_SIZE_X;
  SrcR.h = K_MAP_SIZE_Y;

  DestR.x = 0;
  DestR.y = 0;
  DestR.w = sz(K_DISPLAY_SIZE_X);
  DestR.h = sz(K_DISPLAY_SIZE_Y);

  SDL_RenderCopy(renderer, texture, &SrcR, &DestR);
}