#pragma once

#include <string>

#include "TrGUIObject.hpp"

class TrGUIButton : public TrGUIObject {
 public:
  bool m_mouseInside;
  bool m_pressedInside;
  bool m_releasedInside;
  bool m_wasPressed;
  string m_label;

  TrGUIButton(TrGame *game, SDL_Rect rect, string name);

  void draw() override;
  void update() override;

 private:
  /// Set the color to use in drawing the main part of the button
  /// as the SDL render draw color. This changes based on whether
  /// or not the button is highlighted.
  void setMainButtonDrawColor();
};
