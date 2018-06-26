#pragma once
/**
 * TrGUIResponsiveSystem.hpp
 *
 * Victor Jiao
 *
 * Updates entities responses - highlights, activations, drop-downs, etc.
 */

#include <vector>

#include "../TrGUIComponents/TrGUIHighlightComponent.hpp"
#include "../TrGUIComponents/TrGUIDropdownComponent.hpp"

class TrGUIResponsiveSystem {
 public:
  std::vector<std::shared_ptr<TrGUIHighlightComponent>> m_highlightComponents;
  std::vector<std::shared_ptr<TrGUIDropdownComponent>> m_dropdownComponents;

  void update(TrGame* game) {
    for (auto c : m_highlightComponents) {
      c->update(game);
    }

    for (auto c : m_dropdownComponents) {
      c->update(game);
    }
  }
};

