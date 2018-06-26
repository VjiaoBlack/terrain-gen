/**
 * TrGUIHighlightComponent.cpp
 *
 * Victor Jiao
 *
 * Enables components to have highlights
 */

#include <TrGUI/TrGUIComponents/TrGUIHighlightComponent.hpp>
#include <TrGUI/TrGUIEntity.hpp>

void TrGUIHighlightComponent::update(TrGame *game) {
  if (m_ent->get<TrGUIMouseComponent>()->m_mouseInside) {
    m_ent->get<TrGUIGraphicsComponent>()->m_drawColor = m_highlightColor;
  } else {
    m_ent->get<TrGUIGraphicsComponent>()->m_drawColor =
        m_ent->get<TrGUIGraphicsComponent>()->m_buttonColor;
  }
}

