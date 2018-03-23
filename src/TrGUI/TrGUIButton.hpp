#pragma once

#include "TrGUIObject.hpp"

class TrGUIButton : public TrGUIObject {
 public:
  bool m_pressedInside;
  bool m_releasedInside;
  bool m_wasPressed;
  SDL_Rect m_rect;

  TrGUIButton(TrGame *game, SDL_Rect rect)
      : m_pressedInside(false),
        m_releasedInside(false),
        m_wasPressed(false),
        m_rect(rect) {
    this->m_game = game;
  }

  void draw();
  void update();
};