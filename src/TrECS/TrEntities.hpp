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
  TrActorEntity(string name) : TrEntity<TrActorEntityType>(name) {}

};

class TrBuildingEntity : public TrEntity<TrBuildingEntityType> {
 public:
  TrBuildingEntity(string name) : TrEntity<TrBuildingEntityType>(name) {}

};

class TrPlantEntity : public TrEntity<TrPlantEntityType> {
 public:
  TrPlantEntity(string name) : TrEntity<TrPlantEntityType>(name) {}
};
