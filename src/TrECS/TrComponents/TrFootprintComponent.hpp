#pragma once

#include "../TrComponents.hpp"

/**
 * @brief Buildings have variable footprints, this keeps track of them
 */
class TrFootprintComponent : public TrComponent {
 public:
  int m_w;
  int m_h;
  vector<char> m_cells;

  TrFootprintComponent(int w, int h, vector<char> footprint)
      : m_w(w), m_h(h), m_cells(move(footprint)) {};

  TrFootprintComponent(int w, int h)
      : m_w(w), m_h(h) {};

  void update(TrGame *game, TrEntity *entity) override {};
  virtual void render(TrGame *game, TrEntity *entity);
};
