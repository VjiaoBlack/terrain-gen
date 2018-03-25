/**
 * TrGUIDropdownMenu.cpp
 */

#include "TrGUIDropdownMenu.hpp"
#include "../TrGame.hpp"

TrGUIDropdownMenu::TrGUIDropdownMenu(TrGame* game, TrGUIButton* button,
                                     TrGUIMenu* menu)
    : m_button(button), m_menu(menu), m_isMenuOpen(false) {
  this->m_game = game;
};

TrGUIDropdownMenu::~TrGUIDropdownMenu() {
  delete m_button;
  delete m_menu;
}

void TrGUIDropdownMenu::draw() {
  m_button->draw();
  if (m_isMenuOpen) {
    m_menu->draw();
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
