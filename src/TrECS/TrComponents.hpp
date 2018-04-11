#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

// #include "TrEntities.hpp"
#include "../TrGame.hpp"
class TrEntity;

using namespace std;

/**
 * @brief interface for all components
 * @details using the Entity Component System design pattern
 */
class TrComponent {
 public:
  virtual ~TrComponent(){};
  virtual void update(TrGame* game, TrEntity* entity) = 0;
};
