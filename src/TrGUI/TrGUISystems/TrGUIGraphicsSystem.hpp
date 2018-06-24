#pragma once
/**
 * TrGUIGraphicsSystem.hpp
 *
 * Victor Jiao
 *
 * Draws entities.
 */

#include <vector>
#include <TrGUI/TrGUIComponents/TrGUIHighlightComponent.hpp>

class TrGUIGraphicsSystem {
 public:
  std::vector<TrGUIHighlightComponent*> m_highlightComponents;
};

