#pragma once

/**
 * TrEntitySystem.hpp
 */

#include <vector>
#include <TrECS/TrEntityTypes.hpp>

class TrEntitySystem {
 public:
// TODO: figure out more efficient system
  std::vector<std::unique_ptr<TrPlantEntity>> m_plants;
  std::vector<std::unique_ptr<TrActorEntity>> m_actors;
  std::vector<std::unique_ptr<TrBuildingEntity>> m_buildings;

  TrEntitySystem() {

  }

};
