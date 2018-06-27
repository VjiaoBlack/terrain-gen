#pragma once

/**
 * TrActionComponent.hpp
 */

#include <map>
#include "TrECS/TrItems/TrItem.hpp"
#include "../TrComponents.hpp"

class TrFormula;

using namespace std;

class TrActionComponent : public TrComponent {
 public:

  template<class C>
  void update(TrGame *game, C *entity) {}
};
