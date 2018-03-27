#pragma once

/**
 * TrEntities.hpp
 */

#include "../Game.hpp"
#include "TrComponents.hpp"

/**
 * @brief General interface for all game entities
 */
class TrEntity {
 public:
  TrGame* m_game;

  SDL_Rect m_rect;

  TrGraphicsComponent* m_graphics;

  TrEntity(TrGame* game, SDL_Rect rect, TrGraphicsComponent* graphics)
      : m_game(game), m_rect(rect), m_graphics(graphics) {}
  virtual ~TrEntity() = 0;
  
};

/**
 * @brief Anything that is a building
 */
class TrBuildingEntity : TrEntity {
 public:
  TrBuildingFootprintComponent* m_footprint;

  TrBuildingEntity(){};
  ~TrBuildingEntity() { free m_footprint; }
};

/**
 * @brief Anything that moves and is living
 */
class TrActorEntity : TrEntity {
 public:
  TrPhysicsComponent* m_physics;
  TrPlanningComponent* m_planning;
  TrActorEntity(){};
}