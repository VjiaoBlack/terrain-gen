/**
 * TrMainMenuLoop.cpp
 */

#include "TrMainMenuLoop.hpp"
#include "../TrGame.hpp"
#include "TrGameLoop.hpp"
#include "TrTransitionLoop.hpp"

TrMainMenuLoop::TrMainMenuLoop(const TrGame* game) {
  int score = 10;

  SDL_Surface* textSurface0 =
      TTF_RenderText_Solid(game->m_font, score_text.c_str(), textColor0);
  SDL_Surface* textSurface1 =
      TTF_RenderText_Solid(game->m_font, score_text.c_str(), textColor1);

  SDL_Surface* textSurface2 =
      TTF_RenderText_Solid(game->m_font, score_text.c_str(), textColor2);

  text0 = SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface0);
  text1 = SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface1);
  text2 = SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface2);
  text_width = textSurface1->w;
  text_height = textSurface1->h;
  SDL_FreeSurface(textSurface0);
  SDL_FreeSurface(textSurface1);
  SDL_FreeSurface(textSurface2);
}

TrRenderLoop* TrMainMenuLoop::update(const TrGame* game) {
  game->m_map->update(game->m_keysDown);

  // TODO: implement loading games
  for (auto key : game->m_keysDown) {
    switch (key) {
      case SDLK_v:
        TrTransitionLoop* transition = new TrTransitionLoop(game);
        transition->setTarget(new TrGameLoop(game));
        transition->setSource(this);
        return transition;
        break;
    }
  }

  return NULL;
}

void TrMainMenuLoop::render(const TrGame* game) {
  renderTextureWithOffset(game->m_SDLRenderer, game->m_mapTexture, game->m_xOff,
                          game->m_yOff, game->c_pixelSize);

  SDL_Rect fillRect = {0, 0, sz(K_DISPLAY_SIZE_X), sz(K_DISPLAY_SIZE_Y)};

  SDL_SetRenderDrawColor(game->m_SDLRenderer, 0x00, 0x00, 0x00, 0x40);

  SDL_RenderFillRect(game->m_SDLRenderer, &fillRect);

  // text
  SDL_Rect renderQuad = {
      sz(-4 + K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
      sz(-4 + K_DISPLAY_SIZE_Y / 2 - text_height * K_DISPLAY_SCALE / 2),
      text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
      text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text0, NULL, &renderQuad);

  renderQuad = {
      sz(4 + K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
      sz(4 + K_DISPLAY_SIZE_Y / 2 - text_height * K_DISPLAY_SCALE / 2),
      text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
      text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text2, NULL, &renderQuad);

  renderQuad = {sz(K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
                sz(K_DISPLAY_SIZE_Y / 2 - text_height * K_DISPLAY_SCALE / 2),
                text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
                text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text1, NULL, &renderQuad);
}
