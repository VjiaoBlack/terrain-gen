#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrGame.hpp"

class TrEntity;
using namespace std;

/**
 * @brief interface for all components
 * @details using the Entity Component System design pattern
 */
class TrComponent {
 public:
  virtual ~TrComponent() = default;
  virtual void update(TrGame *game, TrEntity *entity) = 0;
};
