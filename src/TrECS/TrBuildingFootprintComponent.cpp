/**
 * TrBuildingFootprintComponent.cpp
 */

#include "TrBuildingFootprintComponent.hpp"

void TrBuildingFootprintComponent::render(TrGame* game,
                                          TrBuildingEntity* entity) {
  for (int y = 0; y < entity->m_footprint->m_h; y++) {
    for (int x = 0; x < entity->m_footprint->m_w; x++) {
      if (entity->m_footprint->m_cells[x + entity->m_footprint->m_w * y]) {
        SDL_RenderDrawPoint(game->m_SDLRenderer, x + entity->m_rect.x,
                            y + entity->m_rect.y);
      }
    }
  }
}