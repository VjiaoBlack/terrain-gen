#pragma once

/**
 * TrActorEntity.hpp
 */

#include "../TrEntities.hpp"

class TrGame;
class TrActorEntityType;

/**
 * @brief Anything that is a building
 */
class TrActorEntity : public TrEntity {
 public:
  TrActorEntityType *m_type;

  TrActorEntity(TrGame *game, SDL_Rect rect, TrActorEntityType *type)
      : TrEntity(game, rect), m_type(type) {};

  ~TrActorEntity() override = default;
};
