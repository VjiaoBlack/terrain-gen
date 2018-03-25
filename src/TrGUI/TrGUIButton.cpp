/**
 * TrGUIButton.cpp
 */

#include "TrGUIButton.hpp"
#include "../TrGame.hpp"

void TrGUIButton::draw() {
  float sx, sy;

  SDL_RenderGetScale(m_game->m_SDLRenderer, &sx, &sy);
  SDL_RenderSetScale(m_game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));
  SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0xFF);
  SDL_RenderFillRect(m_game->m_SDLRenderer, &m_rect);

  if (m_texture) {
    SDL_RenderCopy(m_game->m_SDLRenderer, m_texture, &m_srcRect, &m_destRect);
  }

  SDL_RenderSetScale(m_game->m_SDLRenderer, sx, sy);
}

void TrGUIButton::update() {
  if (m_releasedInside) {
    m_pressedInside = false;
    m_releasedInside = false;
    m_activated = false;
  }

  SDL_Point mousePos = {m_game->m_mouseX, m_game->m_mouseY};

  if (SDL_PointInRect(&mousePos, &m_rect)) {
    if (m_game->m_buttonsDown.count(SDL_BUTTON_LEFT)) {
      if (false == m_wasPressed) {
        m_pressedInside = true;
      }
    } else if (m_pressedInside) {
      m_releasedInside = true;
      m_activated = true;
    }
  } else {
    m_pressedInside = false;
    m_releasedInside = false;
    m_activated = false;
  }

  if (m_game->m_buttonsDown.count(SDL_BUTTON_LEFT)) {
    m_wasPressed = true;
  } else {
    m_wasPressed = false;
  }
}
