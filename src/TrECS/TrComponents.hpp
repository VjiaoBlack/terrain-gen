#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrGame.hpp"

using std::string;

class TrDummyEntityType {
 public:
  const string msg = "Dummy Entity Type";
};

template<class Parent = TrDummyEntityType>
class TrEntity;

template<class Child, class...Components>
class TrEntityType;


class TrComponent {
 public:
  virtual ~TrComponent() = default;
  void update(TrGame* game, TrEntity<> *entity) {
    std::cout << "Abstract Component" << std::endl;
  }
};
