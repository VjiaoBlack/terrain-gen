
#include "TrWaterMap.hpp"

#include "TrMap.hpp"

void TrWaterMap::update(TrMap* map) {
  bool erosion = true;

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      double m = map->m_height->get(i,j) +
             map->m_water->get(i,j);
      int mi = i;
      int mj = j;
      double mh = m;

      // straight
      if (map->m_height->get(i+1,j) + map->m_water->get(i+1,j) < mh) {
        mi = i+1; mj = j;
        mh = map->m_height->get(i+1,j) + map->m_water->get(i+1,j); }

      if (map->m_height->get(i-1,j) + map->m_water->get(i-1,j) < mh) {
        mi = i-1; mj = j;
        mh = map->m_height->get(i-1,j) + map->m_water->get(i-1,j); }

      if (map->m_height->get(i,j+1) + map->m_water->get(i,j+1) < mh) {
        mi = i; mj = j+1;
        mh = map->m_height->get(i,j+1) + map->m_water->get(i,j+1); }

      if (map->m_height->get(i,j-1) + map->m_water->get(i,j-1) < mh) {
        mi = i; mj = j-1;
        mh = map->m_height->get(i,j-1) + map->m_water->get(i,j-1); }

      // diagonal
      if (m - (map->m_height->get(i+1,j+1) + map->m_water->get(i+1,j+1)) > (m - mh) * 1.4142) {
        mi = i+1; mj = j+1;
        mh = map->m_height->get(i+1,j+1) + map->m_water->get(i+1,j+1); }

      if (m - (map->m_height->get(i-1,j-1) + map->m_water->get(i-1,j-1)) > (m - mh) * 1.4142) {
        mi = i-1; mj = j-1;
        mh = map->m_height->get(i-1,j-1) + map->m_water->get(i-1,j-1); }

      if (m - (map->m_height->get(i+1,j-1) + map->m_water->get(i+1,j-1)) > (m - mh) * 1.4142) {
        mi = i+1; mj = j-1;
        mh = map->m_height->get(i+1,j-1) + map->m_water->get(i+1,j-1); }

      if (m - (map->m_height->get(i-1,j+1) + map->m_water->get(i-1,j+1)) > (m - mh) * 1.4142) {
        mi = i-1; mj = j+1;
        mh = map->m_height->get(i-1,j+1) + map->m_water->get(i-1,j+1); }

      if (mh < m - 0.0001) {
        double diff = m-mh;
        diff *= 0.5;

        // prevents water from cutting too deep
        diff *= 1.0 - map->m_water->get(i,j);
        if (diff > map->m_water->get(i,j)) {
          diff = map->m_water->get(i,j);
        }

        m_water_temp->at(mi,mj) += diff;
        m_water_temp->at(i,j) -= diff;
      }
    }
  }
  // wait or shud we blur the moisture map?

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {

      if (erosion) {
        double change = m_water_temp->at(i,j);
        if (change > 0.0) {
          // adding land
          // TODO: WHAT DO I DO WITH THIS
          change = change * 0.5;
        }
        // printf("%f\n", change);
        // TODO: also look at water differene
        if (map->m_water->at(i,j) > 0.001) {
          // printf("%4f\n", 0.1 / map->m_water->at(i,j));
          change *= abs(change * 0.1 / map->m_water->at(i,j));
        } else {
          change *= abs(change * 40.0);

        }

        map->m_height->at(i  ,j  ) += change /  8.0f;
        map->m_height->at(i+1,j+1) += change / 22.63f;
        map->m_height->at(i+1,j  ) += change / 16.0f;
        map->m_height->at(i+1,j-1) += change / 22.63f;
        map->m_height->at(i  ,j+1) += change / 16.0f;
        map->m_height->at(i  ,j-1) += change / 16.0f;
        map->m_height->at(i-1,j+1) += change / 22.63f;
        map->m_height->at(i-1,j  ) += change / 16.0f;
        map->m_height->at(i-1,j-1) += change / 22.63f;
      }
      map->m_water->at(i,j) += m_water_temp->at(i,j) -.00001;  

      if (map->m_water->at(i,j) < 0.0) {
        map->m_water->at(i,j) = 0.0;
      } 
      if (map->m_water->at(i,j) > 1.0) {
        map->m_water->at(i,j) = 1.0;
      }

      m_water_temp->at(i,j) = 0.0;
    }
  }
}
