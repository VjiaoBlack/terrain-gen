#pragma once
/**
 * TrGUIResponsiveSystem.hpp
 *
 * Victor Jiao
 *
 * Updates entities responses - highlights, activations, drop-downs, etc.
 */

#include <vector>

#include "../TrGUIComponents/TrGUIDropdownComponent.hpp"
#include "../TrGUIComponents/TrGUIHighlightComponent.hpp"

class TrGUIResponsiveSystem {
public:
  std::vector<std::unique_ptr<TrGUIHighlightComponent>> m_highlightComponents;
  std::vector<std::unique_ptr<TrGUIDropdownComponent>> m_dropdownComponents;

  void update(TrGame *game) {
    for (auto &c : m_highlightComponents) {
      c->update(game);
    }

    for (auto &c : m_dropdownComponents) {
      c->update(game);
    }
  }
};
