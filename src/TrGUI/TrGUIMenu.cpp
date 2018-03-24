/**
 * TrGUIMenu.cpp
 */

#include "TrGUIMenu.hpp"
#include "../TrGame.hpp"

void TrGUIMenu::draw() {
  SDL_RenderSetScale(m_game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  // SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  // SDL_RenderDrawRect(m_game->m_SDLRenderer, &m_rect);

  for (auto button : m_buttons) {
    button->draw();
  }

  SDL_RenderSetScale(m_game->m_SDLRenderer, 1, 1);
}

void TrGUIMenu::update() {
  for (auto button : m_buttons) {
    button->update();
  }
}
