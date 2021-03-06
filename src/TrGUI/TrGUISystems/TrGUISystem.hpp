#pragma once

/**
 * TrGUISystem.hpp
 *
 * Victor Jiao
 *
 * Handles a GUI System.
 */

#include <TrGUI/TrGUIComponents/TrGUIContainerComponent.hpp>
#include <list>

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

  std::vector<std::unique_ptr<TrGUIContainerComponent>> m_containerComponents;

  std::list<std::unique_ptr<TrGUIEntity>> m_entities;

  void update(TrGame *game) {
    m_mouse.update(game);
    m_response.update(game);
    m_graphics.update(game);
  }

  void addComponent(std::unique_ptr<TrGUIClickableComponent> c) {
    m_mouse.m_clickableComponents.push_back(std::move(c));
  }

  void addComponent(std::unique_ptr<TrGUIDropdownComponent> c) {
    m_response.m_dropdownComponents.push_back(std::move(c));
  }

  void addComponent(std::unique_ptr<TrGUIGraphicsComponent> c) {
    m_graphics.m_graphicsComponents.push_back(std::move(c));
  }

  void addComponent(std::unique_ptr<TrGUIHighlightComponent> c) {
    m_response.m_highlightComponents.push_back(std::move(c));
  }

  void addComponent(std::unique_ptr<TrGUIMouseComponent> c) {
    m_mouse.m_mouseComponents.push_back(std::move(c));
  }

  void addComponent(std::unique_ptr<TrGUITextComponent> c) {
    m_graphics.m_textComponents.push_back(std::move(c));
  }

  void addComponent(std::unique_ptr<TrGUIContainerComponent> c) {
    m_containerComponents.push_back(std::move(c));
  }
};
