#pragma once
/**
 * TrGUIClickableComponent.hpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include "TrGUIComponent.hpp"
#include "TrGUIMouseComponent.hpp"

class TrGUIClickableComponent : public TrGUIComponent {
 public:
  bool m_pressedInside;
  bool m_releasedInside;
  bool m_wasPressed;
  bool m_activated;

  virtual void update(TrGUIEntity* ent, TrGame* game) {
    if (m_releasedInside) {
      m_pressedInside = false;
      m_releasedInside = false;
      m_activated = false;
    }

    SDL_Point mousePos = {game->m_mouseX, game->m_mouseY};

    if (ent->get<TrGUIMouseComponent>()->m_mouseInside) {
      if (game->m_buttonsDown.count(SDL_BUTTON_LEFT)) {
        if (!m_wasPressed) {
          m_pressedInside = true;
        }
      } else if (m_pressedInside) {
        m_releasedInside = true;
        m_activated = true;
      }
    } else {
      m_pressedInside = false;
      m_releasedInside = false;
      m_activated = false;
    }

    m_wasPressed = game->m_buttonsDown.count(SDL_BUTTON_LEFT) != 0;
  }
};

