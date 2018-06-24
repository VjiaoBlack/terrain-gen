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
#include <TrECS/TrComponents/TrActionComponent.hpp>

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
                                                     TrEntity<TrEntityType<Child, Ts...>>
                                                     *entity) {
  std::get<I>(tuple).template update<TrEntity<TrEntityType<Child, Ts...>>>(game, entity);
};

template<int I, class Child, class...Ts>
enable_if_t<I + 1 < sizeof...(Ts), void> tup_update(tuple<Ts...> &tuple, TrGame *game,
                                                    TrEntity<TrEntityType<Child, Ts...>>
                                                    *entity) {
  std::get<I>(tuple).template update<TrEntity<TrEntityType<Child, Ts...>>>(game, entity);
  tup_update<I + 1, Child, Ts...>(tuple, game, entity);
};



class TrActorEntity;
class TrBuildingEntity;
class TrPlantEntity;


template<class Child, class...Components>
class TrEntityType {
 public:
  string m_typeName;
  tuple<Components...> m_components;

  TrEntityType() {}
  TrEntityType(string name) {}
  TrEntityType(string name, Components... cs) : m_typeName(name) {
    m_components = tuple<Components...>(cs...);
  }

  void update(TrGame *game, TrEntity<TrEntityType<Child, Components...>> *entity) {
    tup_update<0, Child, Components...>(m_components, game, entity);
  }

  shared_ptr<Child> make(TrGame* game) {
    return make_shared<Child>(game, m_typeName);
  }
};

using TrActorEntityType = TrEntityType<TrActorEntity,
                                       TrGraphicsComponent,
                                       TrPhysicsComponent,
                                       TrPlanningComponent,
                                       TrFootprintComponent,
                                       TrActionComponent>;
using TrBuildingEntityType = TrEntityType<TrBuildingEntity,
                                          TrGraphicsComponent,
                                          TrFootprintComponent>;
using TrPlantEntityType = TrEntityType<TrPlantEntity,
                                       TrGraphicsComponent,
                                       TrFootprintComponent>;

