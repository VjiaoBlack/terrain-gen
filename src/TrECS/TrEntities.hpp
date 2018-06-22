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
  TrActorEntity(TrGame* game, string name) : TrEntity<TrActorEntityType>(game, name) {}

};

class TrBuildingEntity : public TrEntity<TrBuildingEntityType> {
 public:
  TrBuildingEntity(TrGame* game, string name) : TrEntity<TrBuildingEntityType>(game, name) {}

};

class TrPlantEntity : public TrEntity<TrPlantEntityType> {
 public:
  TrPlantEntity(TrGame* game, string name) : TrEntity<TrPlantEntityType>(game, name) {}
};
