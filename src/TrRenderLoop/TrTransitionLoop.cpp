/**
 * TrTransitionLoop.cpp
 */

#include "TrTransitionLoop.hpp"
#include "../TrGame.hpp"

class TrMainMenuLoop;


TrRenderLoop* TrTransitionLoop::update(TrGame* game) {
  if (m_waitTick && m_waitTick++ < m_maxWaitTick) {
    return this;
  }
  if (m_waitTick >= m_maxWaitTick) {
    m_waitTick = 0;
  }

  if (m_curTick++ < m_maxTicks) {
    if (m_curTick == m_maxTicks / 2) {
      m_waitTick++;
    }
    return this;
  }

  delete this;
  return nullptr;
}

void TrTransitionLoop::render(TrGame* game) {
  int mx;
  int my;

  if (m_waitTick) {
    return;
  }

  // getting coarser
  if (m_curTick < m_maxTicks / 2) {
    mx = 20 +
         (K_MAP_SIZE_X - 20) * ((float)((m_maxTicks / 2) - m_curTick) /
                                (float)((m_maxTicks / 2)));
    my = 20 +
         (K_MAP_SIZE_Y - 20) * ((float)((m_maxTicks / 2) - m_curTick) /
                                (float)((m_maxTicks / 2)));
  } else {
    // getting finer
    mx = 20 +
         (K_MAP_SIZE_X - 20) * ((float)(m_curTick - (m_maxTicks / 2)) /
                                (float)((m_maxTicks / 2)));
    my = 20 +
         (K_MAP_SIZE_Y - 20) * ((float)(m_curTick - (m_maxTicks / 2)) /
                                (float)((m_maxTicks / 2)));
  }

  // TODO: is this slow?
  SDL_Texture* tempTexSmall =
      SDL_CreateTexture(game->m_SDLRenderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, mx, my);
  SDL_Texture* tempTexFull = SDL_CreateTexture(
      game->m_SDLRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
      sz(K_DISPLAY_SIZE_X), sz(K_DISPLAY_SIZE_Y));

  SDL_SetTextureBlendMode(tempTexSmall, SDL_BLENDMODE_BLEND);
  SDL_SetTextureBlendMode(tempTexFull, SDL_BLENDMODE_BLEND);

  // TODO: make sure mapsize - m_curtick * whatev isnt negative
  SDL_Rect tmp = {0, 0, mx, my};
  SDL_Rect tmpFr = {0, 0, mx, my};
  SDL_Rect screenQuad = {0, 0, sz((K_MAP_SIZE_X)*K_DISPLAY_SCALE),
                         sz((K_MAP_SIZE_Y)*K_DISPLAY_SCALE)};

  SDL_SetRenderTarget(game->m_SDLRenderer, tempTexFull);

  // if over halfway done, render the new one??
  if (m_curTick >= m_maxTicks / 2) {
    m_target->render(game);

  } else {
    m_source->render(game);
  }

  SDL_SetRenderTarget(game->m_SDLRenderer, tempTexSmall);

  SDL_RenderCopy(game->m_SDLRenderer, tempTexFull, nullptr, &tmp);

  float alpha = m_curTick;
  if (alpha >= m_maxTicks / 2) {
    alpha = m_maxTicks - m_curTick;
  }

  alpha = (float)alpha / (float)(m_maxTicks / 2);

  alpha = sqrt(1.0 - alpha) * 0.9 + 0.1;

  alpha *= 255.0;

  SDL_SetRenderTarget(game->m_SDLRenderer, nullptr);
  SDL_SetTextureAlphaMod(tempTexSmall, (int)alpha);

  SDL_RenderCopy(game->m_SDLRenderer, tempTexSmall, &tmpFr, &screenQuad);

  SDL_DestroyTexture(tempTexSmall);
}
