/**
 * TrFootprintComponent.cpp
 */

#include "TrFootprintComponent.hpp"
#include "../TrEntities.hpp"

void TrFootprintComponent::render(TrGame *game, TrEntity *entity) {
  for (int y = 0; y < m_h; y++) {
    for (int x = 0; x < m_w; x++) {
      if (m_cells[x + m_w * y]) {
        SDL_RenderDrawPoint(game->m_SDLRenderer, x + entity->m_rect.x,
                            y + entity->m_rect.y);
      }
    }
  }
}