/**
 * TrGUIMenu.cpp
 */

#include "TrGUIMenu.hpp"
#include "../TrGame.hpp"

void TrGUIMenu::draw() {
  float sx, sy;
  SDL_RenderGetScale(m_game->m_SDLRenderer, &sx, &sy);

  SDL_RenderSetScale(m_game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  SDL_RenderCopy(m_game->m_SDLRenderer, m_labelTexture, &m_srcRect,
                 &m_destRect);

  for (auto button : m_buttons) {
    button->draw();
  }

  // SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  // SDL_RenderDrawRect(m_game->m_SDLRenderer, &m_rect);

  SDL_RenderSetScale(m_game->m_SDLRenderer, sx, sy);
}

void TrGUIMenu::update() {
  for (auto button : m_buttons) {
    button->update();
  }
}
