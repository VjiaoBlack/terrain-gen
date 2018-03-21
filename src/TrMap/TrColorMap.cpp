
#include "TrColorMap.hpp"
#include "TrMap.hpp"

void TrColorMap::update(TrMap* map) {
  switch (map->m_renderState) {
    case 0:
      this->updateGrayscale(map);
      break;

    case 1:
      this->updateHistogram(map);
      break;

    case 2:
      this->updateDisplay(map);
      break;

    case 3:
    default:
      this->updateMoistureDemo(map);
      break;
  }
}


void TrColorMap::updateGrayscale(TrMap* map) {
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // render land
      this->set(i,j,multiplyColor(0xFFFFFFFF, map->m_height->at(i,j), map->m_height->at(i,j), map->m_height->at(i,j)));
    }
  }
}

void TrColorMap::updateHistogram(TrMap* map) {
  // buffer counts in the top row, set to 0 later.
  
  for (int j = 0; j < m_cols; j++) {
    this->set(0,j, 0);
  }

  for (int i = 1; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      int col = m_cols - map->m_height->at(i,j) * m_cols;

      this->at(i,j) = 0;
      this->at(0,col)++;
    }
  }

  double cur_col = 0.0;
  for (int j = 0; j < m_cols; j++) {
    double amt = (double) this->at(0,j) / (double) m_cols;
    if (floor(cur_col + amt) > floor(cur_col)) {
      if (cur_col < m_cols) {
        for (int r = j * m_rows / m_cols; r < m_rows; r++) {
          this->set(r,(int)floor(cur_col), 0xFFFFFFFF);
        }
      }
    } 

    cur_col += amt ;
  }


  for (int j = 0; j < m_cols; j++) {
    this->set(0,j, 0xFF000000);
  }
}

void TrColorMap::updateDisplay(TrMap* map) {

  int threshold[9];
  // -: deep 
  threshold[0] = 0.20 * 256; 
  threshold[1] = 0.40 * 256; 
  threshold[2] = 0.45 * 256; // sand
  threshold[3] = 0.49 * 256; // grass
  threshold[4] = 0.55 * 256; 
  threshold[5] = 0.60 * 256; 
  threshold[6] = 0.68 * 256; // dark mountain level
  threshold[7] = 0.70 * 256;
  threshold[8] = 0.80 * 256;
  // 1: snow


  uint32_t colors[9] = {
    0xFF1A2B56,
    0xFF253C78,
    0xFF3A5BAA,
    0xFFEEDDBB,
    0xFF77BC49,
    0xFF58A327,
    0xFF28771F,
    0xFF210E04,
    0xFF5B3F31};

  // Vec3 light(0.4, 0.2, 0.8);
  // Vec3 light(0.0, 1.0, 0.0);
  Vec3 light(-0.25, -0.25, 0.9071);
  light.normalize();

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // render land
      
      for (int c = 0; c < 9; c++) {
        if (map->m_height->get(i,j) * 255 < threshold[c]) {
          this->set(i,j,colors[c]);
          break;
        }

        if (map->m_height->get(i,j) * 255 >= threshold[8]) {
          this->set(i,j,0xFFEEEEEE);
          break;
        }
      }

      // // moisture lerping
      // if (map->m_height->at(i,j)  * 255 > threshold[2] && map->m_moisture->at(i,j) < 0.1) {
      //   this->at(i,j) = lerpColor(
      //               lerpColor(0xFFEEDDBB, 0xFF5B3F31, (map->m_height->at(i,j) - 0.458) * 0.8), 
      //                    this->at(i,j), 0.2 + 0.8 * map->m_moisture->at(i,j) * 10.0);
      // }
      float wat = 0.6;
      
      uint32_t cur = map->m_height->m_terrace.get(i,j);
      if (map->m_height->m_terrace.get(i-1,j) < cur ||
          map->m_height->m_terrace.get(i,j-1) < cur) {
        wat = 0.8;
      }

      if (map->m_height->m_terrace.get(i-2,j  ) > cur ||
          map->m_height->m_terrace.get(i  ,j-2) > cur ||
          map->m_height->m_terrace.get(i-1,j-1) > cur ||
          map->m_height->m_terrace.get(i-2,j-1) > cur ||
          map->m_height->m_terrace.get(i-1,j-2) > cur) {
        wat = 0.45;
      }

      if (map->m_height->m_terrace.get(i-1,j) > cur ||
          map->m_height->m_terrace.get(i,j-1) > cur) {
        wat = 0.15;
      }
      // if (map->m_height->m_terrace.get(i,j-1) > cur) {
      //   wat = 0.4;
      // }


      // render water
      if (map->m_water->m_water_avg->at(i,j) > 0.001 && map->m_height->at(i,j)  * 255 > threshold[2]) {
        float height = map->m_water->m_water_avg->at(i,j) + map->m_height->at(i,j);
        this->at(i,j) = 0xFF305090;

        int rip = floor(height * 160.0) - 64;
        this->at(i,j) = shiftColor(this->at(i,j), rip, rip, rip); 
      } else {
        wat *= light.dot(map->m_normal->at(i,j));

        // if (map->m_height->at(i,j) * 255  < threshold[2]) {
        //   wat = 0.6;
        // }
        

        this->at(i,j) = multiplyColor(this->at(i,j), wat + 0.4, wat + 0.4, wat + 0.4);
      }
    }
  }
}

void TrColorMap::updateMoistureDemo(TrMap* map) {
  // no initial water colors,just land.
  uint32_t c_wetdirt = 0xFF664433;
  uint32_t c_drydirt = 0xFFBB8866;
  // uint32_t c_sand = 0xFFEEDDBB;
  uint32_t c_grass = 0xFF67AC40;
  uint32_t c_forest = 0xFF58A327;
  uint32_t c_rainforest = 0xFF28771F;

  uint32_t c_rock = 0xFFAB9F91;

  // Vec3 light(0.4, 0.2, 0.8);
  Vec3 light(-0.25, -0.25, 0.9071);
  light.normalize();

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // errr how do I do this....

      if (map->m_height->at(i,j) > 0.7) {
        this->at(i,j) = c_rock;
      } else {
        if (map->m_moisture->at(i,j) > 0.05) {
          this->at(i,j) = lerpColor(c_drydirt, c_wetdirt, 0.4 + map->m_moisture->at(i,j) * 0.6);
        } else {
          this->at(i,j) = c_drydirt;
        }

        if (map->m_vegetation->at(i,j) > 0.2 && map->m_moisture->at(i,j) > 0.3) {
          this->at(i,j) = c_grass;
        }

        if (map->m_vegetation->at(i,j) > 0.65 && map->m_moisture->at(i,j) > 0.4) {
          this->at(i,j) = c_forest;
        }

        if (map->m_vegetation->at(i,j) > 0.8 && map->m_moisture->at(i,j) > 0.8) {
          this->at(i,j) = c_rainforest;
        }
      }

      // render water
      if (map->m_water->m_water_avg->at(i,j) > 0.001) {
        float height = map->m_water->m_water_avg->at(i,j) + map->m_height->at(i,j);
        this->at(i,j) = 0xFF305090;

        int rip = floor(height * 128.0) - 32;
        this->at(i,j) = shiftColor(this->at(i,j), rip, rip, rip); 
      } else {
        float wat = 0.6 * light.dot(map->m_normal->at(i,j));
        this->at(i,j) = multiplyColor(this->at(i,j), wat + 0.4, wat + 0.4, wat + 0.4);
      }
    }
  }
}
