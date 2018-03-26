#pragma once

/**
 * TrTransitionLoop.hpp
 */

#include "TrRenderLoop.hpp"

class TrTransitionLoop : public TrRenderLoop {
 private:
  TrRenderLoop* m_target;
  TrRenderLoop* m_source;
  // TODO: initialize here or in ctor?
  int m_maxTicks = 80;
  int m_curTick = 0;
  int m_maxWaitTick = 20;
  int m_waitTick = 0;

 public:
  TrTransitionLoop(TrGame* game, TrRenderLoop* source, TrRenderLoop* target);
  virtual ~TrTransitionLoop(){};

  virtual TrRenderLoop* update(TrGame* game);
  virtual void render(TrGame* game);
  inline void setTarget(TrRenderLoop* target) { m_target = target; }
  inline void setSource(TrRenderLoop* source) { m_source = source; }

  // TODO: make a stack of TrRenderLoops inside of TrGame
};