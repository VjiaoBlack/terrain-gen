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

template<class C>
void TrFootprintComponent::update(TrGame *game, C *entity) {
  for (int y = 0; y < m_h; y++) {
    for (int x = 0; x < m_w; x++) {
//      if (m_cells[x + m_w * y]) {
//        SDL_RenderDrawPoint(game->m_SDLRenderer, x + entity->m_rect.x,
//                            y + entity->m_rect.y);
//      }
    }
  }
}

//template TrFootprintComponn