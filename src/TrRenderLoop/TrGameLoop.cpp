/**
 * TrGameLoop.cpp
 */

#include "TrGameLoop.hpp"
#include "../TrGame.hpp"
#include "TrTransitionLoop.hpp"

class TrMainMenuLoop;

TrGameLoop::TrGameLoop(TrGame* game) {}

TrRenderLoop* TrGameLoop::update(TrGame* game) {
  game->m_map->update(game->m_keysDown);

  for (auto key : game->m_keysDown) {
    switch (key) {
      case SDLK_v:
        TrTransitionLoop* transition = new TrTransitionLoop(game);
        transition->setTarget(new TrMainMenuLoop(game));
        transition->setSource(this);
        return transition;
        break;
    }
  }

  return nullptr;
}

void TrGameLoop::render(TrGame* game) {
  renderTextureWithOffset(game->m_SDLRenderer, game->m_mapTexture, game->m_xOff,
                          game->m_yOff, game->c_pixelSize);
}
