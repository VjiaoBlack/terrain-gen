#pragma once
/**
 * TrGUIMouseComponent.hpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include "TrGUIComponent.hpp"
#include "TrGUIGraphicsComponent.hpp"

class TrGUIMouseComponent : public TrGUIComponent {
 public:
  bool m_mouseInside;

  virtual void update(TrGame* game);
};

