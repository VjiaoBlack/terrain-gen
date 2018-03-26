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
  TrGUIMenu* m_menu;
  SDL_Texture* m_map;

  TrGameLoop(TrGame* game);
  virtual ~TrGameLoop();

  virtual TrRenderLoop* update(TrGame* game);
  virtual void render(TrGame* game);
};