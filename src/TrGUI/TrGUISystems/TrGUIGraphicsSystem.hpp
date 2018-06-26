#pragma once
/**
 * TrGUIGraphicsSystem.hpp
 *
 * Victor Jiao
 *
 * Draws entities.
 */

#include <vector>
#include "../TrGUIComponents/TrGUITextComponent.hpp"
#include "../TrGUIComponents/TrGUIGraphicsComponent.hpp"

class TrGame;

class TrGUIGraphicsSystem {
 public:
  std::vector<std::shared_ptr<TrGUITextComponent>> m_textComponents;
  std::vector<std::shared_ptr<TrGUIGraphicsComponent>> m_graphicsComponents;

  void update(TrGame* game) {
    for (auto c : m_graphicsComponents) {
      c->update(game);
    }

    for (auto c : m_textComponents) {
      c->update(game);
    }
  }
};

