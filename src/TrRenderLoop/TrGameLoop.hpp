#pragma once

/**
 * TrGameLoop.hpp
 */

#include "../TrGUI/TrGUIDropdownMenu.hpp"
#include "TrRenderLoop.hpp"

class TrGameLoop : public TrRenderLoop {
 public:
  TrGUIMenu* m_menu;

  TrGameLoop(TrGame* game);
  virtual ~TrGameLoop(){};

  virtual TrRenderLoop* update(TrGame* game);
  virtual void render(TrGame* game);
};