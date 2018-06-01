/**
 * TrFootprintComponent.cpp
 */

#include "TrFootprintComponent.hpp"
#include "../TrEntities.hpp"

template<class C>
void TrFootprintComponent::update(TrGame *game, C *entity) {
  for (int y = 0; y < m_h; y++) {
    for (int x = 0; x < m_w; x++) {
      if (m_cells[x + m_w * y]) {
        SDL_RenderDrawPoint(game->m_SDLRenderer, x + entity->m_rect.x,
                            y + entity->m_rect.y);
      }
    }
  }
}