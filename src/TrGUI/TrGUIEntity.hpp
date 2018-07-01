#pragma once
/**
 * TrGUIEntity.hpp
 *
 * Victor Jiao
 *
 * Each GUI object with be a TrGUIEntity with various Components.
 *
 * I rely on virtual functions because GUI objects are usually not
 * computationally
 * the limiting factor of the game (there's not usually many GUI objects on
 * screen).
 */

#include <TrGUI/TrGUIComponents/TrGUIComponent.hpp>
#include <TrGUI/TrGUIComponents/TrGUIContainerComponent.hpp>
#include <TrGUI/TrGUIComponents/TrGUIGraphicsComponent.hpp>
#include <TrGUI/TrGUIComponents/TrGUITextComponent.hpp>
#include <TrGUI/TrGUISystems/TrGUISystem.hpp>
#include <exception>
#include <map>
#include <stdexcept>

class TrMissingComponentException : public std::runtime_error {
public:
  TrMissingComponentException(const string &__arg) : runtime_error(__arg) {}
};

class TrGUIEntity {
public:
  // add component pointers to this tuple later
  // TODO: restrict m_components access
  std::map<string, TrGUIComponent *> m_components;
  TrGUISystem *m_GUISystem;

  TrGUIEntity() = delete;
  TrGUIEntity(TrGUISystem *GUISystem) { m_GUISystem = GUISystem; }
  template <class T> void addComponent(unique_ptr<T> &&component) {
    component->m_ent = this;
    m_components[typeid(T).name()] = component.get();
    if (m_GUISystem) {
      m_GUISystem->addComponent(std::move(component));
    }
  }

  template <class T> T *get() {
    if (!m_components.count(typeid(T).name())) {
      string msg = "missing component: ";
      msg += typeid(T).name();
      throw TrMissingComponentException(msg);
    }
    return static_cast<T *>(m_components[typeid(T).name()]);
  }

  static TrGUIEntity *addButton(TrGame *game, TrGUISystem *system,
                                SDL_Rect rect, std::string label) {
    SDL_Rect textRect = rect;

    auto entity = std::make_unique<TrGUIEntity>(system);

    auto graphicsComp = std::make_unique<TrGUIGraphicsComponent>(rect);
    auto textComp =
        std::make_unique<TrGUITextComponent>(game, label, textRect);
    auto mouseComp = std::make_unique<TrGUIMouseComponent>();
    auto highlightComp = std::make_unique<TrGUIHighlightComponent>();
    auto clickComp = std::make_unique<TrGUIClickableComponent>();

    entity->addComponent(std::move(graphicsComp));
    entity->addComponent(std::move(textComp));
    entity->addComponent(std::move(mouseComp));
    entity->addComponent(std::move(highlightComp));
    entity->addComponent(std::move(clickComp));

    auto pt = entity.get();
    system->m_entities.push_back(std::move(entity));
    return pt;
  }

  static TrGUIEntity *addVerticalMenu(TrGame *game, TrGUISystem *system,
                                      SDL_Rect rect,
                                      std::vector<std::string> &&labels) {
    SDL_Rect textRect = rect;

    auto entity = std::make_unique<TrGUIEntity>(system);

    auto containerComp = std::make_unique<TrGUIContainerComponent>();

    double height =
        static_cast<double>(rect.h -
                            containerComp->m_spacing * (labels.size() - 1)) /
        static_cast<double>(labels.size());
    for (int i = 0; i < labels.size(); i++) {
      SDL_Rect buttonRect = {
          rect.x, static_cast<int>(
                      round(rect.y + i * (height + containerComp->m_spacing))),
          rect.w, static_cast<int>(round(height))};
      containerComp->m_buttons.push_back(
          addButton(game, system, buttonRect, std::move(labels[i])));
    }

    entity->addComponent(std::move(containerComp));

    auto pt = entity.get();
    system->m_entities.push_back(std::move(entity));
    return pt;
  }

  static TrGUIEntity *addHorizontalMenu(TrGame *game, TrGUISystem *system,
                                        SDL_Rect rect,
                                        std::vector<std::string> &&labels) {
    SDL_Rect textRect = rect;

    auto entity = std::make_unique<TrGUIEntity>(system);

    auto containerComp = std::make_unique<TrGUIContainerComponent>();

    double width =
        static_cast<double>(rect.w -
                            containerComp->m_spacing * (labels.size() - 1)) /
        static_cast<double>(labels.size());
    for (int i = 0; i < labels.size(); i++) {
      SDL_Rect buttonRect = {
          static_cast<int>(
              round(rect.x + i * (width + containerComp->m_spacing))),
          rect.y, static_cast<int>(round(width)), rect.h};
      containerComp->m_buttons.push_back(
          addButton(game, system, buttonRect, std::move(labels[i])));
    }

    entity->addComponent(std::move(containerComp));

    auto pt = entity.get();
    system->m_entities.push_back(std::move(entity));
    return pt;
  }
};
