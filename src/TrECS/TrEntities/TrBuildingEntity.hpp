#pragma once

/**
 * TrBuildingEntity.hpp
 */

#include "../TrComponents/TrGraphicsComponent.hpp"
#include "../TrEntities.hpp"

class TrBuildingEntityType;

/**
 * @brief Anything that is a building
 */
class TrBuildingEntity : public TrEntity {
 public:
  TrBuildingEntityType* m_type;

  TrBuildingEntity(TrGame* game, SDL_Rect rect, TrBuildingEntityType* type)
      : TrEntity(game, rect), m_type(type){};

  ~TrBuildingEntity() override = default;
};
