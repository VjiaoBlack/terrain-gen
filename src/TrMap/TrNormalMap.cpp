
#include "TrNormalMap.hpp"
#include "TrMap.hpp"

void TrNormalMap::update(TrMap* map) {
  // go through the map
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // l->r
      Vec3<double> lr(2.0, 0.0, 255.0 * (map->m_height->at(i, j + 1) -
                                         map->m_height->at(i, j - 1)));
      // u->d
      Vec3<double> ud(0.0, 2.0, 255.0 * (map->m_height->at(i + 1, j) -
                                         map->m_height->at(i - 1, j)));

      // // l->r
      // Vec3<double>lr(2.0, 0.0, 255.0 * (map->m_height->at(i,j+1) +
      // map->m_water->at(i,j+1) -
      //                            map->m_height->at(i,j-1) -
      //                            map->m_water->at(i,j-1)));
      // // u->d
      // Vec3<double>ud(0.0, 2.0, 255.0 * (map->m_height->at(i+1,j) +
      // map->m_water->at(i+1,j) -
      //                            map->m_height->at(i-1,j) -
      //                            map->m_water->at(i-1,j)));

      this->at(i, j) = Vec3<double>::cross(lr, ud);
      this->at(i, j).normalize();
    }
  }
}