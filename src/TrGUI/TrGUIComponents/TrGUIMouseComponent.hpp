#pragma once
/**
 * TrGUIMouseComponent.hpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include "TrGUI/TrGUIEntity.hpp"
#include "TrGUIComponent.hpp"
#include "TrGUIGraphicsComponent.hpp"

class TrGUIMouseComponent : public TrGUIComponent {
 public:
  bool m_mouseInside;

  virtual void update(TrGUIEntity* ent, TrGame* game) {
    SDL_Point mousePos = {game->m_mouseX, game->m_mouseY};

    if (SDL_PointInRect(&mousePos, &ent->get<TrGUIGraphicsComponent>()->m_rect)) {
      m_mouseInside = true;
    } else {
      m_mouseInside = false;
    }
  }
};

