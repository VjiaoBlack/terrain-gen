#pragma once
/** 
 * TrEntityTypes.hpp
 *  
 * Victor Jiao
 *
 * Holds all the TrEntityType classes
 */

//#include "TrEntityFactory.hpp"

#include <TrECS/TrComponents/TrGraphicsComponent.hpp>
#include <TrECS/TrComponents/TrPhysicsComponent.hpp>
#include <TrECS/TrComponents/TrPlanningComponent.hpp>
#include <TrECS/TrComponents/TrFootprintComponent.hpp>




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



class MyActorEntity;
class MyBuildingEntity;
class MyPlantEntity;


template<class Child, class...Components>
class MyEntityType {
 public:
  string m_typeName;
  tuple<Components...> m_components;

  MyEntityType(string name) {}
  MyEntityType(string name, Components... cs) : m_typeName(name) {
    m_components = tuple<Components...>(cs...);
  }

  void update(TrGame *game, MyEntity<MyEntityType<Child, Components...>> *entity) {
    tup_update<0, Child, Components...>(m_components, game, entity);
  }

  shared_ptr<Child> make() {
    return make_shared<Child>(m_typeName);
  }
};

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


