#pragma once

/**
 * TrBuildingEntity.hpp
 */

#include "TrBuildingGraphicsComponent.hpp"
#include "TrEntities.hpp"

class TrBuildingFootprintComponent;

/**
 * @brief Anything that is a building
 */
class TrBuildingEntity : public TrEntity {
 public:
  TrBuildingFootprintComponent* m_footprint;

  TrBuildingEntity(TrGame* game, SDL_Rect rect,
                   TrBuildingGraphicsComponent* graphics,
                   TrBuildingFootprintComponent* footprint)
      : TrEntity(game, rect, graphics), m_footprint(footprint){};

  ~TrBuildingEntity() { free(m_footprint); }
};
