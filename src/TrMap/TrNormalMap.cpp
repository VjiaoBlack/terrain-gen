
#include "TrNormalMap.hpp"
#include "TrMap.hpp"

void TrNormalMap::update(TrMap *map) {
  // go through the map
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // l->r
      dvec3 lr(2.0, 0.0, 255.0 * map->m_height->gaussDx(i, j));
      // u->d
      dvec3 ud(0.0, 2.0, 255.0 * map->m_height->gaussDy(i, j));

      this->at(i, j) = cross(lr, ud);
      this->at(i, j) = normalize(this->at(i, j));
    }
  }
}