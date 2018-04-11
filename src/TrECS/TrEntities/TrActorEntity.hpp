#pragma once

/**
 * TrBuildingEntity.hpp
 */

#include "../TrComponents/TrPhysicsComponent.hpp"
#include "../TrComponents/TrPlanningComponent.hpp"
#include "../TrEntities.hpp"

/**
 * @brief Anything that moves and is living
 */
class TrActorEntity : public TrEntity {
 public:
  TrPhysicsComponent* m_physics;
  TrPlanningComponent* m_planning;
  TrActorEntity(TrGame* game, SDL_Rect rect, TrGraphicsComponent* graphics,
                TrPhysicsComponent* physics, TrPlanningComponent* planning)
      : TrEntity(game, rect, graphics),
        m_physics(physics),
        m_planning(planning){};
};