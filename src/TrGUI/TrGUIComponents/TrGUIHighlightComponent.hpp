#pragma once
/**
 * TrGUIHighlightComponent.hpp
 *
 * Victor Jiao
 *
 * Enables components to have highlights
 */

#include "TrGUIComponent.hpp"
#include "TrGUIMouseComponent.hpp"
#include <memory>

class TrGUIHighlightComponent : public TrGUIComponent {
 public:
  SDL_Color m_highlightColor;

  virtual void update(TrGUIEntity *ent, TrGame *game) {
    if (ent->get<TrGUIMouseComponent>()->m_mouseInside) {
      ent->get<TrGUIGraphicsComponent>()->m_drawColor = m_highlightColor;
    } else {
      ent->get<TrGUIGraphicsComponent>()->m_drawColor =
          ent->get<TrGUIGraphicsComponent>()->m_buttonColor;
    }
  }
};

