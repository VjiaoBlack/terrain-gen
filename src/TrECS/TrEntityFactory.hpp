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

#include "TrEntities.hpp"
//#include "../TrGame.hpp"

class TrGame;

using std::tuple;
using std::enable_if_t;
using std::shared_ptr;
using std::make_shared;



//class MyDummyEntityType;
//
//
//template<class Parent = MyDummyEntityType>
//class MyEntity;
//
//template<class Child, class...Components>
//class MyEntityType;



/**
 * Sadly, CLion doesn't support constexpr.
 * Otherwise, you could implement this with just a constexpr.
 */
template<int I, class...Ts>
enable_if_t<I + 1 == sizeof...(Ts), void> tup_print(tuple<Ts...> &tuple) {
  std::cout << I << " " << std::get<I>(tuple) << std::endl;
};

template<int I, class...Ts>
enable_if_t<I + 1 < sizeof...(Ts), void> tup_print(tuple<Ts...> &tuple) {
  std::cout << I << " " << std::get<I>(tuple) << std::endl;
  tup_print<I + 1, Ts...>(tuple);
};

template<int I, class Child, class...Ts>
enable_if_t<I + 1 == sizeof...(Ts), void> tup_update(tuple<Ts...> &tuple, TrGame *game,
                                                     MyEntity<MyEntityType<Child, Ts...>>
                                                     *entity) {
  std::get<I>(tuple).template update<MyEntity<MyEntityType<Child, Ts...>>>(game, entity);
};

template<int I, class Child, class...Ts>
enable_if_t<I + 1 < sizeof...(Ts), void> tup_update(tuple<Ts...> &tuple, TrGame *game,
                                                    MyEntity<MyEntityType<Child, Ts...>>
                                                    *entity) {
  std::get<I>(tuple).template update<MyEntity<MyEntityType<Child, Ts...>>>(game, entity);
  tup_update<I + 1, Child, Ts...>(tuple, game, entity);
};

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



template<class Parent>
class MyEntity {
 public:
  SDL_Rect m_rect;
  MyEntity() {}

  void update(TrGame *game) {
    cout << "Updating entity with data == " << endl;
    Parent::update(game, this);
  }
};

template<class Child, class...Components>
class MyEntityType {
 public:
  static tuple<Components...> components;

  MyEntityType() {}

  static void update(TrGame *game, MyEntity<MyEntityType<Child, Components...>> *entity) {
    tup_update<0, Child, Components...>(components, game, entity);
  }

  static shared_ptr<Child> make() {
    return make_shared<Child>();
  }
};

template<class Child, class...Components>
tuple<Components...> MyEntityType<Child, Components...>::components;



