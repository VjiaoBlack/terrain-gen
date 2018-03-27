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
  bool m_deleteSource;
  TrTransitionLoop(TrGame* game, TrRenderLoop* source, TrRenderLoop* target,
                   bool deleteSource)
      : m_target(target), m_source(source), m_deleteSource(deleteSource){};

 public:
  virtual ~TrTransitionLoop() {
    if (m_deleteSource) delete m_source;
  };

  static TrTransitionLoop* makePushLoop(TrGame* game, TrRenderLoop* source,
                                        TrRenderLoop* target) {
    return new TrTransitionLoop(game, source, target, false);
  }

  static TrTransitionLoop* makePopLoop(TrGame* game, TrRenderLoop* source,
                                       TrRenderLoop* target) {
    return new TrTransitionLoop(game, source, target, true);
  }

  virtual TrRenderLoop* update(TrGame* game);
  virtual void render(TrGame* game);
  inline void setTarget(TrRenderLoop* target) { m_target = target; }
  inline void setSource(TrRenderLoop* source) { m_source = source; }

  // TODO: make a stack of TrRenderLoops inside of TrGame
};