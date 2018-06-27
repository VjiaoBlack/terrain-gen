#pragma once
/**
 * TrGUIGraphicsSystem.hpp
 *
 * Victor Jiao
 *
 * Draws entities.
 */

#include "../TrGUIComponents/TrGUIGraphicsComponent.hpp"
#include "../TrGUIComponents/TrGUITextComponent.hpp"
#include <vector>

class TrGame;

class TrGUIGraphicsSystem {
public:
  std::vector<std::unique_ptr<TrGUITextComponent>> m_textComponents;
  std::vector<std::unique_ptr<TrGUIGraphicsComponent>> m_graphicsComponents;

  void update(TrGame *game) {
    for (auto &c : m_graphicsComponents) {
      c->update(game);
    }

    for (auto &c : m_textComponents) {
      c->update(game);
    }
  }
};
