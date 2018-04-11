#pragma once

/**
 * TrBuildingGraphicsComponent.hpp
 */

#include "../TrComponents.hpp"
#include "TrGraphicsComponent.hpp"

class TrBuildingEntity;
/**
 * @brief handles building graphics
 */
class TrBuildingGraphicsComponent : public TrGraphicsComponent {
 public:
  TrBuildingGraphicsComponent(SDL_Texture* texture);
  TrBuildingGraphicsComponent();

  virtual ~TrBuildingGraphicsComponent();

  /**
   * @brief draws the given entity
   */
  using TrGraphicsComponent::update;
  virtual void update(TrGame* game, TrBuildingEntity* entity);
};