#pragma once

/**
 * TrPlantEntity.hpp
 */

#include "../TrEntities.hpp"

class TrPlantEntityType;
class TrGame;

/**
 * @brief Anything that is a building
 */
class TrPlantEntity : public TrEntity {
 public:
  TrPlantEntityType *m_type;

  TrPlantEntity(TrGame *game, SDL_Rect rect, TrPlantEntityType *type)
      : TrEntity(game, rect), m_type(type) {};

  ~TrPlantEntity() override = default;
};
