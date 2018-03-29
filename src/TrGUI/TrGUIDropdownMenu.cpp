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

  if (m_button->m_activated) {
    m_isMenuOpen = !m_isMenuOpen;
  } else if (m_isMenuOpen && !m_game->m_buttonsDown.count(SDL_BUTTON_LEFT) &&
             m_game->m_buttonsDownPrev.count(SDL_BUTTON_LEFT)) {
    m_isMenuOpen = false;
  }
}
