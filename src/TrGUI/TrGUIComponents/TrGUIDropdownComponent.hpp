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
  std::unique_ptr<TrGUIEntity> m_button;

  virtual void update(TrGame* game) {
    // TODO
  }
};

