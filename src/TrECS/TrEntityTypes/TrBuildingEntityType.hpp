#pragma once

/**
 * TrBuildingEntityType.hpp
 */

#include "TrEntityType.hpp"
#include "../TrComponents/TrBuildingGraphicsComponent.hpp"
#include "../TrComponents/TrFootprintComponent.hpp"

/**
 * @brief Anything that is a building
 */
class TrBuildingEntityType : public TrEntityType {
 public:
  TrFootprintComponent* m_footprint;
  TrGraphicsComponent* m_graphics;

  TrBuildingEntityType(TrBuildingGraphicsComponent* graphics,
                       TrFootprintComponent* footprint)
      : TrEntityType(graphics, footprint) {};

  virtual ~TrBuildingEntityType() { 

  }
};
