#pragma once

/**
 * TrBuildingEntityType.hpp
 */

#include "../TrComponents/TrFootprintComponent.hpp"
#include "../TrComponents/TrGraphicsComponent.hpp"
#include "TrEntityType.hpp"

/**
 * @brief Anything that is a building
 */
class TrBuildingEntityType : public TrEntityType {
 public:
  TrFootprintComponent* m_footprint;

  TrBuildingEntityType(TrGraphicsComponent* graphics,
                       TrFootprintComponent* footprint)
      : TrEntityType(graphics), m_footprint(footprint){};

  ~TrBuildingEntityType() override { delete m_footprint; }
};
