#pragma once
/** 
 * TrEntityTypes.hpp
 *  
 * Victor Jiao
 *
 * Holds all the TrEntityType classes
 */

#include "TrEntityFactory.hpp"

#include <TrECS/TrEntities/TrActorEntity.hpp>
#include <TrECS/TrEntities/TrBuildingEntity.hpp>
#include <TrECS/TrEntities/TrPlantEntity.hpp>

#include <TrECS/TrComponents/TrGraphicsComponent.hpp>
#include <TrECS/TrComponents/TrPhysicsComponent.hpp>
#include <TrECS/TrComponents/TrPlanningComponent.hpp>
#include <TrECS/TrComponents/TrFootprintComponent.hpp>

class MyActorEntity;
class MyBuildingEntity;
class MyPlantEntity;


using MyActorEntityType = MyEntityType<MyActorEntity,
                                       TrGraphicsComponent,
                                       TrPhysicsComponent,
                                       TrPlanningComponent>;
using MyBuildingEntityType = MyEntityType<MyBuildingEntity,
                                          TrGraphicsComponent,
                                          TrFootprintComponent>;
using MyPlantEntityType = MyEntityType<MyPlantEntity,
                                       TrGraphicsComponent,
                                       TrFootprintComponent>;


class MyActorEntity : public MyEntity<MyActorEntityType> {
// public:
//  int data;
};

class MyBuildingEntity : public MyEntity<MyBuildingEntityType> {
// public:
//  int data;
};

class MyPlantEntity : public MyEntity<MyPlantEntityType> {
// public:
//  int data;
};


//
//using TrActorType = TrAbstractEntityType<TrActorEntity,
//                                         TrGraphicsComponent,
//                                         TrPhysicsComponent,
//                                         TrPlanningComponent>;
//
//using TrBuildingType = TrAbstractEntityType<TrBuildingEntity,
//                                         TrGraphicsComponent,
//                                         TrFootprintComponent>;
//
//using TrPlantType = TrAbstractEntityType<TrPlantEntity,
//                                         TrGraphicsComponent,
//                                         TrFootprintComponent>;