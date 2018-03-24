/**
 * TrGUIButton.cpp
 */

#include "TrGUIButton.hpp"
#include "../TrGame.hpp"

void TrGUIButton::draw() {
  SDL_RenderSetScale(m_game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));
  SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0xFF);
  SDL_RenderFillRect(m_game->m_SDLRenderer, &m_rect);

  SDL_RenderCopy(m_game->m_SDLRenderer, m_labelTexture, nullptr, &m_textRect);

  SDL_RenderSetScale(m_game->m_SDLRenderer, 1, 1);
}

void TrGUIButton::update() {
  if (m_releasedInside) {
    m_pressedInside = false;
    m_releasedInside = false;
  }

  SDL_Point mousePos = {m_game->m_mouseX, m_game->m_mouseY};

  if (SDL_PointInRect(&mousePos, &m_rect)) {
    if (m_game->m_buttonsDown.count(SDL_BUTTON_LEFT)) {
      if (false == m_wasPressed) {
        m_pressedInside = true;
      }
    } else if (m_pressedInside) {
      m_releasedInside = true;
    }
  } else {
    m_pressedInside = false;
    m_releasedInside = false;
  }

  if (m_game->m_buttonsDown.count(SDL_BUTTON_LEFT)) {
    m_wasPressed = true;
  } else {
    m_wasPressed = false;
  }
}
