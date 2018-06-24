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
#include <tuple>

class TrGUIEntity {
 private:
  // add components to this tuple later
  std::tuple<> m_components;

};

