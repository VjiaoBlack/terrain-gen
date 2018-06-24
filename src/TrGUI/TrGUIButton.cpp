/**
 * TrGUIButton.cpp
 */

#include "TrGUIButton.hpp"
TrGUIButton::TrGUIButton(TrGame *game, SDL_Rect rect, string name)
    : m_mouseInside(false),
      m_pressedInside(false),
      m_releasedInside(false),
      m_wasPressed(false),
      m_label(std::move(std::move(name))) {
  this->m_game = game;
  this->m_rect = rect;
  SDL_Surface *textSurface = TTF_RenderText_Solid(
      game->m_menuFont.get(), m_label.c_str(), {60, 55, 20, 255});
  m_destRect.w = textSurface->w;
  m_destRect.h = textSurface->h;
  
  // Calculate based on this inner bevel rect so that text doesn't overlap
  // the bevel
  SDL_Rect innerBevelRect = insetRect(m_rect, 1, 1, 1, 1);
  m_destRect.x = innerBevelRect.x + (innerBevelRect.w - m_destRect.w) / 2;
  m_destRect.y = innerBevelRect.y + (innerBevelRect.h - m_destRect.h) / 2;

  m_srcRect = m_destRect;
  m_srcRect.x = 0;
  m_srcRect.y = 0;

  m_texture.reset(SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface));
  SDL_FreeSurface(textSurface);
}

void TrGUIButton::setMainButtonDrawColor() {
  if (m_mouseInside) {
    SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xFF, 0xE9, 0xBC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0xFF);
  }
}

void TrGUIButton::draw() {
  float sx, sy;
  SDL_Rect m_shadowRect = insetRect(m_rect, 1, 1, 0, 0);
  SDL_Rect m_lightRect = insetRect(m_rect, 0, 0, 1, 1);
  SDL_Rect m_innerBevelRect = insetRect(m_rect, 1, 1, 1, 1);

  SDL_RenderGetScale(m_game->m_SDLRenderer, &sx, &sy);
  SDL_RenderSetScale(m_game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  setMainButtonDrawColor();
  SDL_RenderFillRect(m_game->m_SDLRenderer, &m_rect);

  if (m_mouseInside) {
    SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0x88);
  } else {
    SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0x58, 0x3E, 0x09, 0x88);
  }
  SDL_RenderFillRect(m_game->m_SDLRenderer, &m_shadowRect);

  if (m_mouseInside) {
      SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xFF, 0xFF, 0xDC, 0x88);
  } else {
      SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xFF, 0xE9, 0xBC, 0x88);
  }
  SDL_RenderFillRect(m_game->m_SDLRenderer, &m_lightRect);
  
  setMainButtonDrawColor();
  SDL_RenderFillRect(m_game->m_SDLRenderer, &m_innerBevelRect);

  if (m_texture) {
    SDL_RenderCopy(m_game->m_SDLRenderer, m_texture.get(), &m_srcRect, &m_destRect);
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
    m_mouseInside = true;
    if (m_game->m_buttonsDown.count(SDL_BUTTON_LEFT)) {
      if (!m_wasPressed) {
        m_pressedInside = true;
      }
    } else if (m_pressedInside) {
      m_releasedInside = true;
      m_activated = true;
    }
  } else {
    m_mouseInside = false;
    m_pressedInside = false;
    m_releasedInside = false;
    m_activated = false;
  }

  m_wasPressed = m_game->m_buttonsDown.count(SDL_BUTTON_LEFT) != 0;
}
