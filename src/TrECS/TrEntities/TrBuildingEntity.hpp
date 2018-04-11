#pragma once

/**
 * TrBuildingEntity.hpp
 */

#include "../TrComponents/TrBuildingGraphicsComponent.hpp"
#include "../TrComponents/TrGraphicsComponent.hpp"
#include "../TrEntities.hpp"

class TrFootprintComponent;

/**
 * @brief Anything that is a building
 */
class TrBuildingEntity : public TrEntity {
 public:
  TrGraphicsComponent* m_graphics;
  TrFootprintComponent* m_footprint;

  TrBuildingEntity(TrGame* game, SDL_Rect rect,
                   TrBuildingGraphicsComponent* graphics,
                   TrFootprintComponent* footprint)
      : TrEntity(game, rect), m_graphics(graphics), m_footprint(footprint){};

  ~TrBuildingEntity() { free(m_footprint); }
};
