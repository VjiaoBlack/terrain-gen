#pragma once
/**
 * TrGUIClickableComponent.hpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include "TrGUIComponent.hpp"
#include "TrGUIMouseComponent.hpp"

class TrGUIClickableComponent : public TrGUIComponent {
 public:
  bool m_pressedInside;
  bool m_releasedInside;
  bool m_wasPressed;
  bool m_activated;

  virtual void update(TrGame* game);
};

