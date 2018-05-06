#pragma once

/**
 * TrGameLoop.hpp
 */

#include "../TrGUI/TrGUIDropdownMenu.hpp"
#include "../TrGame.hpp"
#include "TrRenderLoop.hpp"

using namespace std;

 class TrGameLoop : public TrRenderLoop, public std::enable_shared_from_this<TrGameLoop> {
 public:
  TrGUIMenu* m_menu;
  SDL_Texture* m_map;

  explicit TrGameLoop(TrGame* game);

  std::shared_ptr<TrGameLoop> getptr() {
    return std::enable_shared_from_this<TrGameLoop>::shared_from_this();
  }
  ~TrGameLoop() override;

  TrRenderLoop* update(TrGame* game) override;
  void render(TrGame* game) override;
};