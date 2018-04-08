/**
 * TrMainMenuLoop.cpp
 */

#include "TrMainMenuLoop.hpp"
#include "../TrGame.hpp"
#include "TrGameLoop.hpp"
#include "TrTransitionLoop.hpp"


TrMainMenuLoop::TrMainMenuLoop(TrGame* game) {
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

  vector<string> labels = {"New Game", "Load Game", "Quit"};

  m_menu = TrGUIMenu::MakeVerticalMenu(
      game,
      (SDL_Rect){sz(K_DISPLAY_SIZE_X / 2 / K_DISPLAY_SCALE - 120 / 2),
                 sz(K_DISPLAY_SIZE_Y / 2 / K_DISPLAY_SCALE - text_height / 2),
                 sz(120), sz(K_DISPLAY_SIZE_Y / 2 / K_DISPLAY_SCALE)},
      labels);
}

TrMainMenuLoop::~TrMainMenuLoop() {
  delete m_menu;
  SDL_DestroyTexture(text0);
  SDL_DestroyTexture(text1);
  SDL_DestroyTexture(text2);
}

TrRenderLoop* TrMainMenuLoop::update(TrGame* game) {
  game->m_map->update(game->m_keysDown);
  m_menu->update();

  if (m_menu->m_buttons[0]->m_activated) {
    game->m_gameStateStack.push_back(new TrGameLoop(game));
    game->m_gameStateTransition = TrTransitionLoop::makePushLoop(
        game, this, game->m_gameStateStack.back());
  }

  if (m_menu->m_buttons[2]->m_activated) {
    game->m_quit = true;
  }

  return nullptr;
}

void TrMainMenuLoop::render(TrGame* game) {
  renderTextureWithOffset(game->m_SDLRenderer, game->m_mapTexture, game->m_xOff,
                          game->m_yOff, K_DISPLAY_SCALE);

  SDL_Rect fillRect = {0, 0, sz(K_DISPLAY_SIZE_X), sz(K_DISPLAY_SIZE_Y)};

  SDL_SetRenderDrawColor(game->m_SDLRenderer, 0x00, 0x00, 0x00, 0x40);

  SDL_RenderFillRect(game->m_SDLRenderer, &fillRect);

  // text
  SDL_Rect renderQuad = {
      sz(-4 + K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
      sz(-4 + K_DISPLAY_SIZE_Y / 4 - text_height * K_DISPLAY_SCALE / 2),
      text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
      text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text0, nullptr, &renderQuad);

  renderQuad = {
      sz(4 + K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
      sz(4 + K_DISPLAY_SIZE_Y / 4 - text_height * K_DISPLAY_SCALE / 2),
      text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
      text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text2, nullptr, &renderQuad);

  renderQuad = {sz(K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
                sz(K_DISPLAY_SIZE_Y / 4 - text_height * K_DISPLAY_SCALE / 2),
                text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
                text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text1, nullptr, &renderQuad);

  m_menu->draw();
}
