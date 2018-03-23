#pragma once

/**
 * TrGameLoop.hpp
 */

#include "TrRenderLoop.hpp"

class TrGameLoop : public TrRenderLoop {
 public:
  TrGameLoop(const TrGame* game);
  virtual ~TrGameLoop(){};

  virtual TrRenderLoop* update(const TrGame* game);
  virtual void render(const TrGame* game);
};