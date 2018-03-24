#pragma once

#include <string>

#include "TrGUIObject.hpp"

class TrGUIButton : public TrGUIObject {
 public:
  bool m_pressedInside;
  bool m_releasedInside;
  bool m_wasPressed;
  SDL_Rect m_rect;
  SDL_Texture* m_labelTexture;
  string m_label;

  SDL_Rect m_textRect;

  TrGUIButton(TrGame* game, SDL_Rect rect, string name)
      : m_pressedInside(false),
        m_releasedInside(false),
        m_wasPressed(false),
        m_rect(rect),
        m_label(name) {
    this->m_game = game;

    SDL_Surface* textSurface = TTF_RenderText_Solid(
        game->m_menuFont, m_label.c_str(), {60, 55, 20, 255});

    m_textRect.w = textSurface->w;
    m_textRect.h = textSurface->h;
    m_textRect.x = m_rect.x + (m_rect.w - m_textRect.w) / 2;
    m_textRect.y = m_rect.y + (m_rect.h - m_textRect.h) / 2;

    m_labelTexture =
        SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface);
    SDL_FreeSurface(textSurface);
  }

  void draw();
  void update();
};