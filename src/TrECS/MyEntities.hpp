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
  float m_hunger;
  float m_energy;
  float m_social;

  MyActorEntity(string name) : MyEntity<MyActorEntityType>(name),
                               m_hunger(0.0), m_energy(0.0),
                               m_social(0.0) {}

};

class MyBuildingEntity : public MyEntity<MyBuildingEntityType> {
 public:
  MyBuildingEntity(string name) : MyEntity<MyBuildingEntityType>(name) {}

};

class MyPlantEntity : public MyEntity<MyPlantEntityType> {
 public:
  MyPlantEntity(string name) : MyEntity<MyPlantEntityType>(name) {}
};
