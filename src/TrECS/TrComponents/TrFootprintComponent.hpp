#pragma once

#include "../TrComponents.hpp"
#include "../TrEntities/TrBuildingEntity.hpp"
/**
 * @brief Buildings have variable footprints, this keeps track of them
 */
class TrFootprintComponent : public TrComponent {
 public:
  int m_w;
  int m_h;
  bool* m_cells;

  TrFootprintComponent(int w, int h, bool* footprint)
      : m_w(w), m_h(h), m_cells(footprint){};

  virtual void update(TrGame* game, TrEntity* entity){};
  virtual void render(TrGame* game, TrEntity* entity);
};
