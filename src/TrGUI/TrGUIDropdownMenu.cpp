/**
 * TrGUIDropdownMenu.cpp
 */

#include "TrGUIDropdownMenu.hpp"
#include "../TrGame.hpp"

void TrGUIDropdownMenu::draw() {
  m_button->draw();
  if (m_isMenuOpen) {
    printf("woah\n");
    m_menu->draw();
    printf("done drawing dropdown\n");
  }
}

void TrGUIDropdownMenu::update() {
  m_button->update();
  m_menu->update();

  SDL_Point mousePos = {m_game->m_mouseX, m_game->m_mouseY};

  if (m_isMenuOpen) {
    if (!SDL_PointInRect(&mousePos, &m_menu->m_rect)) {
      m_isMenuOpen = false;
    }
  } else {
    if (m_button->m_activated) {
      m_isMenuOpen = true;
    }
  }
}
