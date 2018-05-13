
#include "TrGraphics.hpp"

int SDL_Deleter::boop = 0;


sdl_texture_pt TrSDL::null_texture() {
  return sdl_texture_pt(nullptr);
}

ttf_font_pt TrSDL::null_font() {
  return ttf_font_pt(nullptr);
}

// TODO: is this even useful anymore?
void renderTextureWithOffset(SDL_Renderer *renderer, SDL_Texture *texture,
                             int xOff, int yOff, int pixelSize) {
  SDL_Rect SrcR;
  SDL_Rect DestR;

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