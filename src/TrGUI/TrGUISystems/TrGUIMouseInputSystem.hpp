#pragma once
/**
 * TrGUIMouseInputSystem.hpp
 *
 * Victor Jiao
 *
 * Registers the changing mouse states of a system
 */

#include <vector>

#include "../TrGUIComponents/TrGUIClickableComponent.hpp"
#include "../TrGUIComponents/TrGUIMouseComponent.hpp"

class TrGUIMouseInputSystem {
public:
  std::vector<std::unique_ptr<TrGUIMouseComponent>> m_mouseComponents;
  std::vector<std::unique_ptr<TrGUIClickableComponent>> m_clickableComponents;

  void update(TrGame *game) {
    for (auto &c : m_mouseComponents) {
      c->update(game);
    }

    for (auto &c : m_clickableComponents) {
      c->update(game);
    }
  }
};
