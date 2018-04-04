
#include "TrWaterMap.hpp"

#include "TrMap.hpp"

void TrWaterMap::rain(TrMap* map) {
  // this->experimentalRain(map);
  // return;

  int m_speed = 1;
  for (int i = 0; i < m_speed * K_MAP_SIZE_X * K_MAP_SIZE_Y / 150; i++) {
    this->at(map->m_yrandDist(map->m_yrandEngine),
             map->m_xrandDist(map->m_xrandEngine)) += 0.001;
  }
}

void TrWaterMap::update(TrMap* map) {
  // this->experimentalUpdate(map);
  // return;
  //
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      map->m_water->m_water_avg->at(i, j) =
          map->m_water->m_water_avg->at(i, j) * 0.95 +
          map->m_water->at(i, j) * 0.05;
      // map->m_water->at(i, j);

      double m = map->m_height->get(i, j) + map->m_water->get(i, j);
      int mi = i;
      int mj = j;
      double mh = m;

      // straight
      if (map->m_height->get(i + 1, j) + map->m_water->get(i + 1, j) < mh) {
        mi = i + 1;
        mj = j;
        mh = map->m_height->get(i + 1, j) + map->m_water->get(i + 1, j);
      }

      if (map->m_height->get(i - 1, j) + map->m_water->get(i - 1, j) < mh) {
        mi = i - 1;
        mj = j;
        mh = map->m_height->get(i - 1, j) + map->m_water->get(i - 1, j);
      }

      if (map->m_height->get(i, j + 1) + map->m_water->get(i, j + 1) < mh) {
        mi = i;
        mj = j + 1;
        mh = map->m_height->get(i, j + 1) + map->m_water->get(i, j + 1);
      }

      if (map->m_height->get(i, j - 1) + map->m_water->get(i, j - 1) < mh) {
        mi = i;
        mj = j - 1;
        mh = map->m_height->get(i, j - 1) + map->m_water->get(i, j - 1);
      }

      // diagonal
      if (m - (map->m_height->get(i + 1, j + 1) +
               map->m_water->get(i + 1, j + 1)) >
          (m - mh) * 1.4142) {
        mi = i + 1;
        mj = j + 1;
        mh = map->m_height->get(i + 1, j + 1) + map->m_water->get(i + 1, j + 1);
      }

      if (m - (map->m_height->get(i - 1, j - 1) +
               map->m_water->get(i - 1, j - 1)) >
          (m - mh) * 1.4142) {
        mi = i - 1;
        mj = j - 1;
        mh = map->m_height->get(i - 1, j - 1) + map->m_water->get(i - 1, j - 1);
      }

      if (m - (map->m_height->get(i + 1, j - 1) +
               map->m_water->get(i + 1, j - 1)) >
          (m - mh) * 1.4142) {
        mi = i + 1;
        mj = j - 1;
        mh = map->m_height->get(i + 1, j - 1) + map->m_water->get(i + 1, j - 1);
      }

      if (m - (map->m_height->get(i - 1, j + 1) +
               map->m_water->get(i - 1, j + 1)) >
          (m - mh) * 1.4142) {
        mi = i - 1;
        mj = j + 1;
        mh = map->m_height->get(i - 1, j + 1) + map->m_water->get(i - 1, j + 1);
      }

      if (mh < m - 0.0001) {
        double diff = m - mh;
        diff *= 0.5;

        // prevents water from cutting too deep
        diff *= 1.0 - map->m_water->get(i, j);
        if (diff > map->m_water->get(i, j)) {
          diff = map->m_water->get(i, j);
        }

        m_water_temp->at(mi, mj) += diff;
        m_water_temp->at(i, j) -= diff;
      }
    }
  }
  // wait or shud we blur the moisture map?

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      if (map->m_erosionState) {
        double change = m_water_temp->at(i, j);
        // double change = 0;
        if (change > 0.0) {
          // adding land
          // TODO: WHAT DO I DO WITH THIS
          change = change * 0.5;
        }
        // printf("%f\n", change);
        // TODO: also look at water differene
        if (map->m_water->at(i, j) > 0.001) {
          // printf("%4f\n", 0.1 / map->m_water->at(i,j));
          change *= abs(change * 0.1 / map->m_water->at(i, j));
        } else {
          change *= abs(change * 40.0);
        }

        map->m_height->at(i, j) += change / 8.0f;
        map->m_height->at(i + 1, j + 1) += change / 22.63f;
        map->m_height->at(i + 1, j) += change / 16.0f;
        map->m_height->at(i + 1, j - 1) += change / 22.63f;
        map->m_height->at(i, j + 1) += change / 16.0f;
        map->m_height->at(i, j - 1) += change / 16.0f;
        map->m_height->at(i - 1, j + 1) += change / 22.63f;
        map->m_height->at(i - 1, j) += change / 16.0f;
        map->m_height->at(i - 1, j - 1) += change / 22.63f;
      }
      map->m_water->at(i, j) += m_water_temp->at(i, j) - .00001;

      if (map->m_water->at(i, j) < 0.0) {
        map->m_water->at(i, j) = 0.0;
      }
      if (map->m_water->at(i, j) > 1.0) {
        map->m_water->at(i, j) = 1.0;
      }

      m_water_temp->at(i, j) = 0.0;
    }
  }
}

// void TrWaterMap::experimentalRain(TrMap* map) {
//   for (int i = 0; i < 1000; i++) {
//     double x = map->m_frandDist(map->m_randEngine);
//     double y = map->m_frandDist(map->m_randEngine);
//     double id = map->m_frandDist(map->m_randEngine);

//     TrWaterPoint w(id, x, y);
//     m_water[id] = w;

//     map->m_water->at(round(x), round(y)) += w.amt;
//   }

// }

// void TrWaterMap::experimentalUpdate(TrMap* map) {
//   // merge... rip

//   // for (auto wp = m_water.begin(); wp != m_water.end(); wp++) {
//   //   int x = round(wp->second.x);
//   //   int y = round(wp->second.y);

//   //   m_water_temp->at(y,x).amt = 0;
//   // }

//   for (int i = 0; i < m_rows; i++) {
//     for (int j = 0; j < m_cols; j++) {
//       m_water_temp->at(i, j) = TrWaterPoint();
//     }
//   }

//   std::unordered_map<double, TrWaterPoint> add_pts;
//   std::unordered_set<double> rm_pts;

//   // put rain into the buffer and merge them
//   for (auto wp = m_water.begin(); wp != m_water.end(); wp++) {
//     // TODO: no rounding
//     //
//     int x = round(wp->second.x);
//     int y = round(wp->second.y);

//     // printf("%d, %d\n", x, y);

//     // TODO: should check this? idk
//     TrWaterPoint w = TrWaterPoint::Merge(m_water_temp->at(y, x), wp->second);

//     m_water_temp->at(y,x) = w;

//   }

//   // printf("done\n");
//   m_water.clear();

//   // printf("wtf\n");
//   for (int i = 0; i < m_rows; i++) {
//     for (int j = 0; j < m_cols; j++) {
//       if (m_water_temp->at(i,j)._id > 0) {
//         m_water[m_water_temp->at(i,j)._id] = m_water_temp->at(i,j);
//       }
//     }
//   }

//   // printf("done!!\n");

//   for (auto wp = m_water.begin(); wp != m_water.end(); wp++) {
//     // TrWaterPoint *w = &(wp->second);
//     wp->second.x += wp->second.vx;
//     wp->second.y += wp->second.vy;

//     // TODO: no rounding
//     int x = round(wp->second.x);
//     int y = round(wp->second.y);

//     double nx = map->m_normal->at(x,y).x;
//     double ny = map->m_normal->at(x,y).y;

//     double len = sqrt(nx * nx + ny * ny);

//     nx /= len;
//     ny /= len;

//     wp->second.vx = ny * 0.1 + 0.9 * wp->second.vx;
//     wp->second.vy = nx * 0.1 + 0.9 * wp->second.vy;
//   }

//   // printf("endc\n");

//   for (int i = 0; i < m_rows; i++) {
//     for (int j = 0; j < m_cols; j++) {
//       map->m_water->at(i, j) = 0.0;
//     }
//   }

//   for (auto wp = m_water.begin(); wp != m_water.end(); wp++) {
//     wp->second.amt -= 0.00001;
//   }

//   std::unordered_set<TrWaterPoint> rm;

//   for (auto wp = m_water.begin(); wp != m_water.end(); wp++) {
//     if (wp->second.amt < 0.0001) {
//       rm.insert(wp->second);
//     }
//   }

//   for (auto w : rm) {
//     m_water.erase(w._id);
//   }

//   for (auto wp : m_water) {
//     map->m_water->at(round(wp.second.x), round(wp.second.y)) +=
//     wp.second.amt;
//   }

// }