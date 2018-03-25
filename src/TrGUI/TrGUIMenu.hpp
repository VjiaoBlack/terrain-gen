#pragma once

#include <string>
#include <vector>

#include "TrGUIButton.hpp"
#include "TrGUIObject.hpp"

using namespace std;
class TrGUIMenu : public TrGUIObject {
 public:
  int m_spacing = 10;
  vector<TrGUIObject *> m_buttons;

  // TODO: how ot call parent constructor
  TrGUIMenu(TrGame *game, SDL_Rect rect, vector<string> labels);

  virtual ~TrGUIMenu();

  void draw();
  void update();
};