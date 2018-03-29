/**
 * TrBuildingGraphicsComponent.cpp
 */

#include "TrBuildingGraphicsComponent.hpp"
#include "TrBuildingEntity.hpp"
#include "TrBuildingFootprintComponent.hpp"

TrBuildingGraphicsComponent::TrBuildingGraphicsComponent(SDL_Texture* texture)
    : TrGraphicsComponent(texture){};
TrBuildingGraphicsComponent::TrBuildingGraphicsComponent()
    : TrBuildingGraphicsComponent(nullptr) {}

TrBuildingGraphicsComponent::~TrBuildingGraphicsComponent(){};

/**
 * @brief draws the given entity
 */
void TrBuildingGraphicsComponent::update(TrGame* game,
                                         TrBuildingEntity* entity) {
  // draw pixels
  float sx, sy;
  SDL_RenderGetScale(game->m_SDLRenderer, &sx, &sy);

  SDL_RenderSetScale(game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  entity->m_footprint->render(game, entity);

  // if (m_texture) {
  //   SDL_RenderCopy(game->m_SDLRenderer, m_texture, &m_srcRect, &m_destRect);

  // } else {
  //   SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
  //   SDL_RenderDrawRect(game->m_SDLRenderer, &m_rect);
  // }
  // for (auto button : m_buttons) {
  //   button->draw();
  // }

  SDL_RenderSetScale(game->m_SDLRenderer, sx, sy);
}
