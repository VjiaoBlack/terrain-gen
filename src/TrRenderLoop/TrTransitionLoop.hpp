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
  TrTransitionLoop(TrGame* game) : m_target(nullptr), m_source(nullptr) {}
  virtual ~TrTransitionLoop(){};

  virtual TrRenderLoop* update(TrGame* game);
  virtual void render(TrGame* game);
  void setTarget(TrRenderLoop* target) { m_target = target; }

  // TODO: setup setSource to store a texture with the source's data
  // 	   instead of the entire source itself.
  void setSource(TrRenderLoop* source) { m_source = source; }
};