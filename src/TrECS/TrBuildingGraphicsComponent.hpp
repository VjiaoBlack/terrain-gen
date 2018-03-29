#pragma once

/**
 * TrBuildingGraphicsComponent.hpp
 */

#include "TrComponents.hpp"

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
  // TODO: is this right
  using TrComponent::update;
  virtual void update(TrGame* game, TrBuildingEntity* entity);
};