#pragma once

/**
 * TrMainMenuLoop.hpp
 */

#include <string>
#include <vector>

#include "../TrGUI/TrGUIMenu.hpp"
#include "TrRenderLoop.hpp"

class TrGame;
class TrGUIEntity;
class TrGUISystem;

class TrMainMenuLoop : public TrRenderLoop {
 public:
  std::string score_text = "ATHENA";
  int score = 10;
  SDL_Color textColor0 = {255, 215, 0, 255};
  SDL_Color textColor1 = {218, 165, 32, 255};
  SDL_Color textColor2 = {139, 69, 19, 255};
  int text_width;
  int text_height;

  sdl_texture_pt text0;
  sdl_texture_pt text1;
  sdl_texture_pt text2;

  unique_ptr<TrGUIMenu> m_menu;

  unique_ptr<TrGUIEntity> m_ent;
  unique_ptr<TrGUISystem> m_GUISystem;

  explicit TrMainMenuLoop(TrGame *game);

  ~TrMainMenuLoop() override;

  TrRenderLoop *update(TrGame *game) override;
  void render(TrGame *game) override;
};