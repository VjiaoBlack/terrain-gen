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
  int m_maxTicks = 100;
  int m_curTick = 0;

 public:
  TrTransitionLoop(const TrGame* game) : m_target(NULL), m_source(NULL) {}
  virtual ~TrTransitionLoop(){};

  virtual TrRenderLoop* update(const TrGame* game);
  virtual void render(const TrGame* game);
  void setTarget(TrRenderLoop* target) { m_target = target; }

  // TODO: setup setSource to store a texture with the source's data
  // 	   instead of the entire source itself.
  void setSource(TrRenderLoop* source) { m_source = source; }
};