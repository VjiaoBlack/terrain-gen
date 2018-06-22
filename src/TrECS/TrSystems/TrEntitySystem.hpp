#pragma once

/**
 * TrEntitySystem.hpp
 */

#include <vector>
#include <TrECS/TrEntityTypes.hpp>

class TrEntitySystem {
 public:
// TODO: figure out more efficient system
  std::vector<shared_ptr<TrPlantEntity>> m_plants;
  std::vector<shared_ptr<TrActorEntity>> m_actors;
  std::vector<shared_ptr<TrBuildingEntity>> m_buildings;

  TrEntitySystem() {

  }

};
