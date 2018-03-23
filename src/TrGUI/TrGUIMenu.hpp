#pragma once

#include <vector>

#include "TrGUIButton.hpp"
#include "TrGUIObject.hpp"

using namespace std;
class TrGUIMenu : public TrGUIObject {
 public:
  const int m_spacing = 10;
  vector<TrGUIButton *> m_buttons;
  SDL_Rect m_rect;

  // TODO: how ot call parent constructor

  TrGUIMenu(TrGame *game, SDL_Rect rect, int numButtons) : m_rect(rect) {
    this->m_game = game;
    m_buttons.resize(numButtons);
    for (int i = 0; i < numButtons; i++) {
      // TODO: make this spacing metric even over all of m_rect
      m_buttons[i] =
          new TrGUIButton(game, {rect.x, rect.y + (rect.h * i / numButtons),
                                 rect.w, rect.h / numButtons - m_spacing});
    }
  }

  virtual ~TrGUIMenu() {
    for (int i = 0; i < m_buttons.size(); i++) {
      delete m_buttons[i];
    }
  }

  void draw();
  void update();
};