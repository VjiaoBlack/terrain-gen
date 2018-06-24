#pragma once
/**
 * TrGUIComponent.hpp
 *
 * Victor Jiao
 *
 * The abstract base class
 */

#include "../../TrGame.hpp"

class TrGUIEntity;

class TrGUIComponent {
 public:
  virtual void update(TrGUIEntity* ent, TrGame* game);
};

