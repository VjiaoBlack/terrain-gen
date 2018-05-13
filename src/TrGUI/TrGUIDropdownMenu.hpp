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

  unique_ptr<TrGUIButton> m_button;
  unique_ptr<TrGUIMenu> m_menu;
  bool m_isMenuOpen;

  // TODO: how ot call parent constructor

  TrGUIDropdownMenu(TrGame *game, unique_ptr<TrGUIButton> button, unique_ptr<TrGUIMenu>
  menu);
  ~TrGUIDropdownMenu() override;

  void draw() override;
  void update() override;
};