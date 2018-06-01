#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrGame.hpp"

using std::string;

class MyDummyEntityType {
 public:
  const string msg = "Dummy Entity Type";
};

template<class Parent = MyDummyEntityType>
class MyEntity;

template<class Child, class...Components>
class MyEntityType;


class MyComponent {
 public:
  virtual ~MyComponent() = default;
  void update(TrGame* game, MyEntity<> *entity) {
    std::cout << "Abstract Component" << std::endl;
  }
};
