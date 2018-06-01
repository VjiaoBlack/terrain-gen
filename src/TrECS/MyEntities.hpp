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

class MyActorEntity : public MyEntity<MyActorEntityType> {
 public:
  MyActorEntity(string name) : MyEntity<MyActorEntityType>(name) {}

};

class MyBuildingEntity : public MyEntity<MyBuildingEntityType> {
 public:
  MyBuildingEntity(string name) : MyEntity<MyBuildingEntityType>(name) {}

};

class MyPlantEntity : public MyEntity<MyPlantEntityType> {
 public:
  MyPlantEntity(string name) : MyEntity<MyPlantEntityType>(name) {}
};
