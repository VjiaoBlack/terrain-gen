#pragma once
/**
 * TrGUIComponent.hpp
 *
 * Victor Jiao
 *
 * The abstract base class
 */

#include <memory>

class TrGUIEntity;
class TrGame;

class TrGUIComponent {
 public:
  TrGUIEntity* m_ent;
  virtual void update(TrGame* game) {};
};

