#pragma once

/**
 * TrTransitionLoop.hpp
 */

#include "TrRenderLoop.hpp"

#include <memory>
#include <utility> #include <utility>
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
  TrTransitionLoop(TrGame *game,
                   shared_ptr<TrRenderLoop> source,
                   shared_ptr<TrRenderLoop> target,
                   bool deleteSource)
      : m_target(std::move(target)), m_source(std::move(source)), m_deleteSource(deleteSource) {};

 public:
  ~TrTransitionLoop() override {
//    if (m_deleteSource) delete m_source;
  };

  static unique_ptr<TrTransitionLoop> makePushLoop(TrGame *game, shared_ptr<TrRenderLoop>
  source,
                                                   shared_ptr<TrRenderLoop> target) {
    return unique_ptr<TrTransitionLoop>(new TrTransitionLoop(game, std::move(source),
                                                             std::move(target), false));
  }

  static unique_ptr<TrTransitionLoop> makePopLoop(TrGame *game, shared_ptr<TrRenderLoop>
  source,
                                                  shared_ptr<TrRenderLoop> target) {
    return unique_ptr<TrTransitionLoop>(new TrTransitionLoop(game, std::move(source),
                                                             std::move(target), true));
  }

  TrRenderLoop *update(TrGame *game) override;
  void render(TrGame *game) override;
  inline void setTarget(shared_ptr<TrRenderLoop> target) { m_target = std::move(target); }
  inline void setSource(shared_ptr<TrRenderLoop> source) { m_source = std::move(source); }

  // TODO: make a stack of TrRenderLoops inside of TrGame
};