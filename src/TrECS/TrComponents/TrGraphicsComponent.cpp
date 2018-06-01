/**
 * TrGraphicsComponent.cpp
 */

#include "TrGraphicsComponent.hpp"

#include "../TrEntities.hpp"

template<class C>
void TrGraphicsComponent::update(TrGame *game, C *entity) {
  float sx, sy;

  SDL_RenderGetScale(game->m_SDLRenderer, &sx, &sy);
  SDL_RenderSetScale(game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));
  if (m_texture) {
    SDL_RenderCopy(game->m_SDLRenderer, m_texture, nullptr, &(entity->m_rect));
  } else {
    SDL_SetRenderDrawColor(game->m_SDLRenderer, m_color.r, m_color.g, m_color.b,
                           m_color.a);
    SDL_RenderFillRect(game->m_SDLRenderer, &(entity->m_rect));
  }
  SDL_RenderSetScale(game->m_SDLRenderer, sx, sy);
}