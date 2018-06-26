#pragma once

/**
 * TrGUISystem.hpp
 *
 * Victor Jiao
 *
 * Handles a GUI System.
 */

#include "TrGUIGraphicsSystem.hpp"
#include "TrGUIMouseInputSystem.hpp"
#include "TrGUIResponsiveSystem.hpp"

class TrGUIClickableComponent;
class TrGUIDropdownComponent;
class TrGUIMouseComponent;

class TrGUISystem {
 public:
  TrGUIGraphicsSystem m_graphics;
  TrGUIMouseInputSystem m_mouse;
  TrGUIResponsiveSystem m_response;

  void update(TrGame* game) {
    m_mouse.update(game);
    m_response.update(game);
    m_graphics.update(game);
  }

  void addComponent(std::shared_ptr<TrGUIClickableComponent> c) {
    m_mouse.m_clickableComponents.push_back(c);
  }

  void addComponent(std::shared_ptr<TrGUIDropdownComponent> c) {
    m_response.m_dropdownComponents.push_back(c);
  }

  void addComponent(std::shared_ptr<TrGUIGraphicsComponent> c) {
    m_graphics.m_graphicsComponents.push_back(c);
  }

  void addComponent(std::shared_ptr<TrGUIHighlightComponent> c) {
    m_response.m_highlightComponents.push_back(c);
  }

  void addComponent(std::shared_ptr<TrGUIMouseComponent> c) {
    m_mouse.m_mouseComponents.push_back(c);
  }

  void addComponent(std::shared_ptr<TrGUITextComponent> c) {
    m_graphics.m_textComponents.push_back(c);
  }
};

