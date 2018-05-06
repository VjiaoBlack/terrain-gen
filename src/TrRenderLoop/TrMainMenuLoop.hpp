#pragma once

/**
 * TrMainMenuLoop.hpp
 */

#include <string>
#include <vector>

#include "../TrGUI/TrGUIMenu.hpp"
#include "TrRenderLoop.hpp"
class TrGame;

class TrMainMenuLoop : public TrRenderLoop, public std::enable_shared_from_this<TrMainMenuLoop> {
 public:
  std::string score_text = "ATHENA";
  int score = 10;
  SDL_Color textColor0 = {255, 215, 0, 255};
  SDL_Color textColor1 = {218, 165, 32, 255};
  SDL_Color textColor2 = {139, 69, 19, 255};
  int text_width;
  int text_height;

  SDL_Texture* text0;
  SDL_Texture* text1;
  SDL_Texture* text2;

  TrGUIMenu* m_menu;

  explicit TrMainMenuLoop(TrGame* game);

  std::shared_ptr<TrMainMenuLoop> getptr() {
    return std::enable_shared_from_this<TrMainMenuLoop>::shared_from_this();
  }

  ~TrMainMenuLoop() override;

  TrRenderLoop* update(TrGame* game) override;
  void render(TrGame* game) override;
};