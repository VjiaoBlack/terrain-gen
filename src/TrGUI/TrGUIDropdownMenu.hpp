#pragma once

#include <string>
#include <vector>

#include "TrGUIButton.hpp"
#include "TrGUIDropdownMenu.hpp"
#include "TrGUIMenu.hpp"
#include "TrGUIObject.hpp"

using namespace std;
class TrGUIDropdownMenu : public TrGUIObject {
 public:
  TrGUIButton* m_button;
  TrGUIMenu* m_menu;
  bool m_isMenuOpen;

  // TODO: how ot call parent constructor

  TrGUIDropdownMenu(TrGUIButton* button, TrGUIMenu* menu)
      : m_button(button), m_menu(menu), m_isMenuOpen(false){};

  virtual ~TrGUIDropdownMenu() {
    delete m_button;
    delete m_menu;
  }

  void draw();
  void update();
};