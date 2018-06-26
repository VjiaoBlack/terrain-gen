/**
 * TrGUIGraphicsComponent.hpp
 *
 * Victor Jiao
 *
 * Enables components to be drawn (at all)
 */

#include <TrGUI/TrGUIComponents/TrGUIGraphicsComponent.hpp>
#include <TrGame.hpp>

void TrGUIGraphicsComponent::update(TrGame *game) {
  float sx, sy;
  SDL_Rect m_shadowRect = makeInsetRect(m_rect, 1, 1, 0, 0);
  SDL_Rect m_lightRect = makeInsetRect(m_rect, 0, 0, 1, 1);
  SDL_Rect m_innerBevelRect = makeInsetRect(m_rect, 1, 1, 1, 1);

  SDL_RenderGetScale(game->m_SDLRenderer, &sx, &sy);
  SDL_RenderSetScale(game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  setRenderDrawColor(game->m_SDLRenderer, m_drawColor);

  SDL_RenderFillRect(game->m_SDLRenderer, &m_rect);

  setRenderDrawColor(game->m_SDLRenderer, m_drawDarkColor);
  SDL_RenderFillRect(game->m_SDLRenderer, &m_shadowRect);

  setRenderDrawColor(game->m_SDLRenderer, m_drawLightColor);
  SDL_RenderFillRect(game->m_SDLRenderer, &m_lightRect);

  setRenderDrawColor(game->m_SDLRenderer, m_drawColor);

  SDL_RenderFillRect(game->m_SDLRenderer, &m_innerBevelRect);

  SDL_RenderSetScale(game->m_SDLRenderer, sx, sy);
}

