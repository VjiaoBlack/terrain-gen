#pragma once

/**
 * TrActorEntityType.hpp
 */

#include "../TrComponents/TrGraphicsComponent.hpp"
#include "../TrComponents/TrPhysicsComponent.hpp"
#include "../TrComponents/TrPlanningComponent.hpp"
#include "TrEntityType.hpp"

/**
 * @brief Anything that is a building
 */
class TrActorEntityType : public TrEntityType {
 public:
  TrPhysicsComponent* m_physics;
  TrPlanningComponent* m_planning;

  TrActorEntityType(TrGraphicsComponent* graphics, TrPhysicsComponent* physics,
                    TrPlanningComponent* planning)
      : TrEntityType(graphics), m_physics(physics), m_planning(planning){};

  virtual ~TrActorEntityType() {}
};
