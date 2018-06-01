#pragma once

/**
 * TrEntitySystem.hpp
 */

#include <vector>
#include <TrECS/TrEntityTypes.hpp>

class TrEntitySystem {
 public:
// TODO: figure out more efficient system
  std::vector<shared_ptr<MyPlantEntity>> m_plants;
  std::vector<shared_ptr<MyActorEntity>> m_actors;
  std::vector<shared_ptr<MyBuildingEntity>> m_buildings;

  TrEntitySystem() {

  }

};
