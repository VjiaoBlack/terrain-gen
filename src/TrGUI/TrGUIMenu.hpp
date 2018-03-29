#pragma once

#include <string>
#include <vector>

#include "TrGUIButton.hpp"
#include "TrGUIObject.hpp"

using namespace std;
class TrGUIMenu : public TrGUIObject {
 private:
  TrGUIMenu(TrGame *game, SDL_Rect rect, vector<string> labels);

 public:
  int m_spacing = 10;
  vector<TrGUIObject *> m_buttons;

  static TrGUIMenu *MakeHorizontalMenu(TrGame *game, SDL_Rect rect,
                                       vector<string> labels);
  static TrGUIMenu *MakeVerticalMenu(TrGame *game, SDL_Rect rect,
                                     vector<string> labels);

  virtual ~TrGUIMenu();

  void draw();
  void update();
};