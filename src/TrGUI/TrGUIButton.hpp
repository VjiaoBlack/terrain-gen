#pragma once

#include <string>

#include "TrGUIObject.hpp"

class TrGUIButton : public TrGUIObject {
 public:
  bool m_pressedInside;
  bool m_releasedInside;
  bool m_wasPressed;
  string m_label;

  TrGUIButton(TrGame* game, SDL_Rect rect, string name)
      : m_pressedInside(false),
        m_releasedInside(false),
        m_wasPressed(false),
        m_label(name) {
    this->m_game = game;
    this->m_rect = rect;

    SDL_Surface* textSurface = TTF_RenderText_Solid(
        game->m_menuFont, m_label.c_str(), {60, 55, 20, 255});

    m_destRect.w = textSurface->w;
    m_destRect.h = textSurface->h;
    m_destRect.x = m_rect.x + (m_rect.w - m_destRect.w) / 2;
    m_destRect.y = m_rect.y + (m_rect.h - m_destRect.h) / 2;

    m_srcRect = m_destRect;
    m_srcRect.x = 0;
    m_srcRect.y = 0;

    m_texture = SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface);
    SDL_FreeSurface(textSurface);
  }

  void draw();
  void update();
};