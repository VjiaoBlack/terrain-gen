#pragma once

/**
 * TrPlantEntityType.hpp
 */

#include "../TrComponents/TrFootprintComponent.hpp"
#include "../TrComponents/TrGraphicsComponent.hpp"
#include "TrEntityType.hpp"

/**
 * @brief Anything that is a building
 */
class TrPlantEntityType : public TrEntityType {
 public:
  TrFootprintComponent* m_footprint;

  TrPlantEntityType(TrGraphicsComponent* graphics,
                    TrFootprintComponent* footprint)
      : TrEntityType(graphics), m_footprint(footprint){};

  ~TrPlantEntityType() override = default;
};
