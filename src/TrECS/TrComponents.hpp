#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrGame.hpp"


class MyDummyEntityType {
 public:
  const string msg = "Dummy Entity Type";
};

template<class Parent = MyDummyEntityType>
class MyEntity;

template<class Child, class...Components>
class MyEntityType;


using namespace std;

/**
 * @brief interface for all components
 * @details using the Entity Component System design pattern
 */
//class TrComponent {
// public:
//  virtual ~TrComponent() = default;
//  virtual void update(TrGame *game, TrEntity *entity) = 0;
//};

class MyComponent {
 public:
  virtual ~MyComponent() = default;
  void update(TrGame* game, MyEntity<> *entity) {
    std::cout << "Abstract Component" << std::endl;
  }
};
