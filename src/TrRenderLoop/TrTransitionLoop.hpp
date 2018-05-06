#pragma once

/**
 * TrTransitionLoop.hpp
 */

#include "TrRenderLoop.hpp"

#include <memory>

class TrTransitionLoop : public TrRenderLoop {
 private:
  shared_ptr<TrRenderLoop> m_target;
  shared_ptr<TrRenderLoop> m_source;
  // TODO: initialize here or in ctor?
  int m_maxTicks = 80;
  int m_curTick = 0;
  int m_maxWaitTick = 20;
  int m_waitTick = 0;
  bool m_deleteSource;
  TrTransitionLoop(TrGame* game, shared_ptr<TrRenderLoop> source, shared_ptr<TrRenderLoop> target,
                   bool deleteSource)
      : m_target(target), m_source(source), m_deleteSource(deleteSource){};

 public:
  ~TrTransitionLoop() override {
//    if (m_deleteSource) delete m_source;
  };

  static TrTransitionLoop* makePushLoop(TrGame* game, shared_ptr<TrRenderLoop> source,
                                        shared_ptr<TrRenderLoop> target) {
    return new TrTransitionLoop(game, source, target, false);
  }

  static TrTransitionLoop* makePopLoop(TrGame* game, shared_ptr<TrRenderLoop> source,
                                       shared_ptr<TrRenderLoop> target) {
    return new TrTransitionLoop(game, source, target, true);
  }

  TrRenderLoop* update(TrGame* game) override;
  void render(TrGame* game) override;
  inline void setTarget(TrRenderLoop* target) { m_target.reset(target); }
  inline void setSource(TrRenderLoop* source) { m_source.reset(source); }

  // TODO: make a stack of TrRenderLoops inside of TrGame
};