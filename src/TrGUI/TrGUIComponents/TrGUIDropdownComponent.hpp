#pragma once
/**
 * TrGUIDropdownComponent.hpp
 *
 * Victor Jiao
 *
 * Makes entities able to "appear" and "disappear"
 */

#include "TrGUIComponent.hpp"
#include <memory>

class TrGUIDropdownComponent : public TrGUIComponent {
// only activates when another TrGUIEntity is clicked
 public:
  std::shared_ptr<TrGUIEntity> m_button;

  virtual void update(TrGUIEntity* ent, TrGame* game) {
    // TODO
  }
};

