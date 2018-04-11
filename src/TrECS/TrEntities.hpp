#pragma once

/**
 * TrEntities.hpp
 */

#include "../TrGame.hpp"
#include "TrComponents.hpp"

#include "TrComponents/TrGraphicsComponent.hpp"
#include "TrComponents/TrPhysicsComponent.hpp"
#include "TrComponents/TrPlanningComponent.hpp"

/**
 * @brief General interface for all game entities
 */
class TrEntity {
 public:
  TrGame* m_game;

  SDL_Rect m_rect;

  TrEntity(TrGame* game, SDL_Rect rect) : m_game(game), m_rect(rect) {}
  virtual ~TrEntity() = 0;
};
