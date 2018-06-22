#pragma once
/** 
 * MyEntities.hpp
 *  
 * Victor Jiao
 *
 * <DETAILS>
 */

#include "TrEntityTypes.hpp"
#include "TrEntityFactory.hpp"

class TrActorEntity : public TrEntity<TrActorEntityType> {
 public:
  double m_hunger;
  double m_energy;
  double m_social;



  TrActorEntity(string name) : TrEntity<TrActorEntityType>(name),
                               m_hunger(0.0), m_energy(0.0),
                               m_social(0.0) {}

};

class TrBuildingEntity : public TrEntity<TrBuildingEntityType> {
 public:
  TrBuildingEntity(string name) : TrEntity<TrBuildingEntityType>(name) {}

};

class TrPlantEntity : public TrEntity<TrPlantEntityType> {
 public:
  TrPlantEntity(string name) : TrEntity<TrPlantEntityType>(name) {}
};
