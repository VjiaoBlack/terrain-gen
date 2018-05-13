#pragma once

/**
 * TrGameLoop.hpp
 */

#include "../TrGUI/TrGUIDropdownMenu.hpp"
#include "../TrGame.hpp"
#include "TrRenderLoop.hpp"

using namespace std;

class TrGameLoop : public TrRenderLoop {
 public:
  unique_ptr<TrGUIMenu> m_menu;
  sdl_texture_pt m_map;

  explicit TrGameLoop(TrGame *game);

  ~TrGameLoop() override;

  TrRenderLoop *update(TrGame *game) override;
  void render(TrGame *game) override;
};