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
#include <TrGUI/TrGUIComponents/TrGUIGraphicsComponent.hpp>
#include <TrGUI/TrGUIComponents/TrGUITextComponent.hpp>
#include <TrGUI/TrGUISystems/TrGUISystem.hpp>

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
    return static_pointer_cast<T>(m_components[typeid(T).name()]);
  }

  static std::shared_ptr<TrGUIEntity> makeButton(TrGame *game, shared_ptr<TrGUISystem> system) {
    auto entity = make_shared<TrGUIEntity>(system);
    auto graphicsComp = make_shared<TrGUIGraphicsComponent>();
    graphicsComp->m_rect.x = 10;
    graphicsComp->m_rect.y = 10;
    graphicsComp->m_rect.w = 100;
    graphicsComp->m_rect.h = 100;
    entity->addComponent(graphicsComp);

    auto textComp = make_shared<TrGUITextComponent>(game, "test");
    entity->addComponent(textComp);

    return entity;
  }

};

