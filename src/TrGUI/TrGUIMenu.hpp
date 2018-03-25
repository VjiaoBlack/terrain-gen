#pragma once

#include <string>
#include <vector>

#include "TrGUIButton.hpp"
#include "TrGUIObject.hpp"

using namespace std;
class TrGUIMenu : public TrGUIObject {
 public:
  int m_spacing = 10;
  vector<TrGUIObject *> m_buttons;

  // TODO: how ot call parent constructor

  TrGUIMenu(TrGame *game, SDL_Rect rect, vector<string> labels) {
    m_rect = {rect.x / K_DISPLAY_SCALE, rect.y / K_DISPLAY_SCALE,
              rect.w / K_DISPLAY_SCALE, rect.h / K_DISPLAY_SCALE};
    this->m_game = game;
    int numButtons = labels.size();

    m_buttons.resize(numButtons);

    for (int i = 0; i < numButtons; i++) {
      // TODO: make this spacing metric even over all of m_rect
      m_buttons[i] = new TrGUIButton(
          game, {m_rect.x, m_rect.y + i * (m_rect.h + m_spacing) / (numButtons),
                 m_rect.w, (m_rect.h + m_spacing) / (numButtons)-m_spacing},
          labels[i]);
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