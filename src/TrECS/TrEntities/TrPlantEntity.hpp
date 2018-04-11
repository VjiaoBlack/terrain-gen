#pragma once

/**
 * TrPlantEntity.hpp
 */

#include "../TrComponents/TrGraphicsComponent.hpp"
#include "../TrEntities.hpp"

class TrPlantEntityType;

/**
 * @brief Anything that is a building
 */
class TrPlantEntity : public TrEntity {
 public:
  TrPlantEntityType* m_type;

  TrPlantEntity(TrGame* game, SDL_Rect rect, TrPlantEntityType* type)
      : TrEntity(game, rect), m_type(type){};

  ~TrPlantEntity() {}
};
