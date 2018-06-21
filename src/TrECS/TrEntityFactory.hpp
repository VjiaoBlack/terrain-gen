#pragma once
/**
 * TrEntityFactory.hpp
 *  
 * Victor Jiao
 *
 * Templated Abstract Factory System
 *
 * EntityType System already eliminates need to have one class per object type
 *   i.e. no need to have TrPlantTree, TrActorGoat, TrActorHuman, etc...
 *        only need objects for object categories
 *
 * However, there still may be several object categories.
 * Keeping track of all of them can be a pain.
 *
 * Thus, this templated Abstract Factory System attempts to move the complexity
 * of storing these slightly different TrThingCategories into template parameters.
 *
 * We can do something like:
 *
 * using TrActorEntityType = TrAbstractEntity<TrGraphicsComponent,
 *                                            TrPhysicsComponent,
 *                                            TrPlanningComponent>;
 *
 * From there, we can use templating to enforce constructors and stuff.
 *
 * We can use the same system for constructing TrFormulas and things like that.
 *
 * This might also be able to alleviate our I/O issue - since all the parameters
 * to these TrThingTypes are within template parameters, we can also use templating
 * to create functions that do I/O.
 *
 * For example, within TrAbstractEntity...
 * We can incorporate an I/O function within TrAbstractEntity
 * that goes through the components in turn and reads in parameters for them in
 * turn.
 * So for TrActorEntityType, we would read in name, color, size, footprint.
 *
 * This presents for us an additional challenge - how can we structure the
 * TrGraphicsComponent to be able to edit the game screen?
 *
 * Should all TrComponents hold a pointer to TrGame?
 *
 * Or should all TrComponent update functions receive a pointer to TrGame?
 *
 * I think the latter might be better, this way we can keep better track of
 * when TrGame is being modified...?
 *
 * Also keep in mind - in the final version, we might be only drawing TrEntities
 * to buffered TrMapData objects within TrMap -
 *
 * That idea in itself (drawing TrEntities to buffered TrMapData objects) suggests
 * that we should have a more streamlined rendering pipeline within TrGame.
 *
 * Update World (water, height, etc)
 *
 * Update Entities (do they move, etc)
 *
 * Draw Entity -> Draw to Buffered Entity Screen
 *   Not all entities move - keep one Entity Screen for nonmoving objects,
 *   one for moving objects.
 *
 * Draw Overall -> Draw to Color Buffer
 *   Use Buffered Screens along with other data (height, water, normals, etc)
 *   (also, light, shadow, etc)
 *   to draw the final colors to color buffer.
 *
 *   Consider using a separate process to compute light intensities in one step.
 *   This might allow us to better use parallelism or maybe we can even offset a
 *   few of these batched actions to GPU for faster speed.
 */

#include <iostream>
#include <tuple>
#include <type_traits>
#include <memory>
#include <vector>
//#include <TrData/TrData.hpp>

#include <TrData/TrData.hpp>

#include "TrEntities.hpp"
//#include "../TrGame.hpp"

class TrGame;

using std::tuple;
using std::enable_if_t;
using std::shared_ptr;
using std::make_shared;


/**
 * How can we force TrEntity to have the right data members and right TrComponents?
 * How can we use the compiler to help us make these declarations?
 */

/**
 * MyComponent stores all the data
 *
 * MyEntityType just stores all the components each MyEntity has
 * It has to be able to actually have some components, in order to
 * be able to flexibly define what precise version of each component each Type has
 *
 * MyEntity refers back to MyEntityType, and uses MyEntityType to initialize
 * its own components.
 */

/**
 * Virtual functions can be slow,
 * but this allows us to store components more easily.
 */

template<class T, class Parent>
enable_if_t<is_same<Parent, MyPlantEntityType>::value, void> _entityUpdate(TrGame
                                                                           *game,
                                                                           T *ent) {
  TrData::m_plantTypes[ent->m_typeName]->update(game, ent);
}

template<class T, class Parent>
enable_if_t<is_same<Parent, MyActorEntityType>::value, void> _entityUpdate(TrGame
                                                                           *game,
                                                                           T *ent) {
  TrData::m_actorTypes[ent->m_typeName]->update(game, ent);
}

template<class Parent>
class MyEntity {
 public:
  string m_typeName; // refers to type name
  SDL_Rect m_rect;

  double m_x;
  double m_y;

  MyEntity(string name) : m_typeName(name) {}

  void update(TrGame *game) {
    _entityUpdate<MyEntity<Parent>, Parent>(game, this);
  }
};

