#pragma once

#include "TrBuildingEntity.hpp"
#include "TrComponents.hpp"
/**
 * @brief Buildings have variable footprints, this keeps track of them
 */
class TrBuildingFootprintComponent : public TrComponent {
 public:
  int m_w;
  int m_h;
  bool* m_cells;

  TrBuildingFootprintComponent(int w, int h, bool* footprint)
      : m_w(w), m_h(h), m_cells(footprint){};

  virtual void render(TrGame* game, TrBuildingEntity* entity);
};
