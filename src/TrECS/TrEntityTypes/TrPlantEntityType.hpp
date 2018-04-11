#pragma once

/**
 * TrPlantEntityType.hpp
 */

#include "../TrComponents/TrGraphicsComponent.hpp"
#include "TrEntityType.hpp"

/**
 * @brief Anything that is a building
 */
class TrPlantEntityType : public TrEntityType {
 public:

  TrPlantEntityType(TrGraphicsComponent* graphics)
      : TrEntityType(graphics) {};

  virtual ~TrPlantEntityType() { }
};
