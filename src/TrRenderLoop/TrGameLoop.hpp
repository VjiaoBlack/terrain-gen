#pragma once

/**
 * TrGameLoop.hpp
 */

#include "TrRenderLoop.hpp"

class TrGameLoop : public TrRenderLoop {
 public:
  TrGameLoop(TrGame* game);
  virtual ~TrGameLoop(){};

  virtual TrRenderLoop* update(TrGame* game);
  virtual void render(TrGame* game);
};