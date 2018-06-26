#pragma once
/**
 * TrGUIContainerComponent.hpp
 *
 * Victor Jiao
 *
 * Makes entities able to have things inside it
 */

#include "TrGUIComponent.hpp"
#include <vector>

class TrGUIEntity;

class TrGUIContainerComponent : public TrGUIComponent {
 public:
  int m_spacing = 10;
  std::vector<TrGUIEntity*> m_buttons;

  virtual void update(TrGame* game) {};
};

