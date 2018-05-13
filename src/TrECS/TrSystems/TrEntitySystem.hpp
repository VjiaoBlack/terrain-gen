#pragma once

/**
 * TrEntitySystem.hpp
 */

#include <vector>

#include "../TrEntities/TrPlantEntity.hpp"
#include "../TrEntities/TrActorEntity.hpp"
#include "../TrEntities/TrBuildingEntity.hpp"

class TrEntitySystem {
 public:
// TODO: figure out more efficient system
  std::vector<unique_ptr<TrPlantEntity>> m_plants;
  std::vector<unique_ptr<TrActorEntity>> m_actors;
  std::vector<unique_ptr<TrBuildingEntity>> m_buildings;

  TrEntitySystem() {

  }



};
