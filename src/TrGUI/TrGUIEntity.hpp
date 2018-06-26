#pragma once
/**
 * TrGUIEntity.hpp
 *
 * Victor Jiao
 *
 * Each GUI object with be a TrGUIEntity with various Components.
 *
 * I rely on virtual functions because GUI objects are usually not computationally
 * the limiting factor of the game (there's not usually many GUI objects on screen).
 */

#include <TrGUI/TrGUIComponents/TrGUIComponent.hpp>
#include <map>
#include <exception>
#include <stdexcept>
#include <TrGUI/TrGUIComponents/TrGUIGraphicsComponent.hpp>
#include <TrGUI/TrGUIComponents/TrGUITextComponent.hpp>
#include <TrGUI/TrGUISystems/TrGUISystem.hpp>

class TrMissingComponentException : public std::runtime_error {
 public:
  TrMissingComponentException(const string &__arg) : runtime_error(__arg) {}
};

class TrGUIEntity {
 public:
  // add component pointers to this tuple later
  // TODO: restrict m_components access
  std::map<string, std::shared_ptr<TrGUIComponent>> m_components;
  std::shared_ptr<TrGUISystem> m_GUISystem;

  TrGUIEntity() = delete;
  TrGUIEntity(std::shared_ptr<TrGUISystem> GUISystem) {
    m_GUISystem = GUISystem;
  }
  template<class T>
  void addComponent(shared_ptr<T> component) {
    component->m_ent = this;
    if (m_GUISystem) {
      m_GUISystem->addComponent(component);
    }
    m_components[typeid(T).name()] = static_pointer_cast<TrGUIComponent>(component);
  }

  template<class T>
  shared_ptr<T> get() {
    if (!m_components.count(typeid(T).name())) {
      string msg = "missing component: ";
      msg += typeid(T).name();
      throw TrMissingComponentException(msg);
    }
    return static_pointer_cast<T>(m_components[typeid(T).name()]);
  }

  static std::shared_ptr<TrGUIEntity> makeButton(TrGame *game, shared_ptr<TrGUISystem> system,
                                                 SDL_Rect rect) {
    SDL_Rect textRect = rect;

    auto entity = make_shared<TrGUIEntity>(system);

    auto graphicsComp = make_shared<TrGUIGraphicsComponent>(rect);
    auto textComp = make_shared<TrGUITextComponent>(game, "test", textRect);
    auto mouseComp = make_shared<TrGUIMouseComponent>();
    auto highlightComp = make_shared<TrGUIHighlightComponent>();
    auto clickComp = make_shared<TrGUIClickableComponent>();

    entity->addComponent(graphicsComp);
    entity->addComponent(textComp);
    entity->addComponent(mouseComp);
    entity->addComponent(highlightComp);
    entity->addComponent(clickComp);

    return entity;
  }

};

