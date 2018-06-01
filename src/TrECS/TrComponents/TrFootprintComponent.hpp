#pragma once

#include "../TrComponents.hpp"

/**
 * @brief Buildings have variable footprints, this keeps track of them
 */

class TrFootprintComponent : public MyComponent {
 public:
  int m_w;
  int m_h;
  vector<char> m_cells;

  TrFootprintComponent(int w, int h, vector<char> footprint)
      : m_w(w), m_h(h), m_cells(move(footprint)) {};

  TrFootprintComponent(int w, int h)
      : m_w(w), m_h(h) {};

  TrFootprintComponent() : TrFootprintComponent(1, 1) {}

  template<class C>
  void update(TrGame *game, C *entity);
};
