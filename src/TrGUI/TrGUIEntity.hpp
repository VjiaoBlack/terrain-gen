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

#include "TrGUIComponents/TrGUIComponent.hpp"
#include <map>

class TrGUIEntity {
 public:
  // add component pointers to this tuple later
  // TODO: restrict m_components access
  std::map<string, TrGUIComponent*> m_components;

  template<class T>
  void addComponent(T* component) {
    m_components[typeid(T).name()] = static_cast<TrGUIComponent*>(component);
  }

  template<class T>
  T* get() {
    return static_cast<T*>(m_components[typeid(T).name()]);
  }
};

