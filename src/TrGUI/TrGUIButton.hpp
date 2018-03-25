#pragma once

#include <string>

#include "TrGUIObject.hpp"

class TrGUIButton : public TrGUIObject {
 public:
  bool m_pressedInside;
  bool m_releasedInside;
  bool m_wasPressed;
  string m_label;

  TrGUIButton(TrGame* game, SDL_Rect rect, string name);

  void draw();
  void update();
};