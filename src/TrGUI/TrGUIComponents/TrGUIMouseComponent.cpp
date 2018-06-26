/**
 * TrGUIMouseComponent.cpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include <TrGUI/TrGUIComponents/TrGUIMouseComponent.hpp>
#include <TrGame.hpp>
#include <TrGUI/TrGUIEntity.hpp>

void TrGUIMouseComponent::update(TrGame* game) {
  SDL_Point mousePos = {game->m_mouseX, game->m_mouseY};

  if (SDL_PointInRect(&mousePos, &m_ent->get<TrGUIGraphicsComponent>()->m_rect)) {
    m_mouseInside = true;
  } else {
    m_mouseInside = false;
  }
}

