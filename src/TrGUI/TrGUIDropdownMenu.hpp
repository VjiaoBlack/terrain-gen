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

  TrGUIDropdownMenu(TrGame* game, TrGUIButton* button, TrGUIMenu* menu);
  ~TrGUIDropdownMenu() override;

  void draw() override;
  void update() override;
};