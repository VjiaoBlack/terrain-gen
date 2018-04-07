
#include "TrColorMap.hpp"
#include "../TrUtils/TrMath.hpp"
#include "../TrUtils/TrSimulation.hpp"
#include "TrMap.hpp"

int LENGTH = 400;
TrColorMap::TrColorMap(int rows, int cols)
    : TrMapData<uint32_t>(rows, cols),
      m_renderState(0),
      ocean(new cOcean(64, 0.0005, vector2(3, 3), LENGTH, true)) {
  m_light = Vec3<double>(0, 0, 1);
  m_light.normalize();
  m_hour = 6;
  m_day = 1;
  m_month = 6;
}
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
      this->set(i, j, multiplyColor(0xFFFFFFFF, map->m_height->at(i, j),
                                    map->m_height->at(i, j),
                                    map->m_height->at(i, j)));
    }
  }
}

void TrColorMap::updateHistogram(TrMap* map) {
  // buffer counts in the top row, set to 0 later.

  for (int j = 0; j < m_cols; j++) {
    this->set(0, j, 0);
  }

  for (int i = 1; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      int col = m_cols - map->m_height->at(i, j) * m_cols;

      this->at(i, j) = 0;
      this->at(0, col)++;
    }
  }

  double cur_col = 0.0;
  for (int j = 0; j < m_cols; j++) {
    double amt = (double)this->at(0, j) / (double)m_cols;
    if (floor(cur_col + amt) > floor(cur_col)) {
      if (cur_col < m_cols) {
        for (int r = j * m_rows / m_cols; r < m_rows; r++) {
          this->set(r, (int)floor(cur_col), 0xFFFFFFFF);
        }
      }
    }

    cur_col += amt;
  }

  for (int j = 0; j < m_cols; j++) {
    this->set(0, j, 0xFF000000);
  }
}

void TrColorMap::updateDisplay(TrMap* map) {
  int threshold[9];
  // -: deep
  threshold[0] = 0.20 * 256;
  threshold[1] = 0.40 * 256;
  threshold[2] = 0.45 * 256;  // sand
  threshold[3] = 0.49 * 256;  // grass
  threshold[4] = 0.55 * 256;
  threshold[5] = 0.60 * 256;
  threshold[6] = 0.68 * 256;  // dark mountain level
  threshold[7] = 0.70 * 256;
  threshold[8] = 0.80 * 256;
  // 1: snow

  // uint32_t colors[9] = {0xFF1A2B56, 0xFF253C78, 0xFF3A5BAA,
  uint32_t colors[9] = {0xFFBB8866, 0xFFBB8866, 0xFFBB8866,
                        0xFFEEDDBB, 0xFF77BC49, 0xFF58A327,
                        0xFF28771F, 0xFF210E04, 0xFF5B3F31};

  clock_t endFrame = clock();
  double calcMs = clockToMilliseconds(endFrame);

  ocean->render(calcMs / 1000, true);

  for (int i = 1; i < m_rows - 1; i++) {
    for (int j = 1; j < m_cols - 1; j++) {
      // render land

      int index = (j % ocean->N) + (i % ocean->N) * ocean->N;
      int indexl = ((j - 1) % ocean->N) + (i % ocean->N) * ocean->N;
      int indexr = ((j + 1) % ocean->N) + (i % ocean->N) * ocean->N;
      int indext = (j % ocean->N) + ((i - 1) % ocean->N) * ocean->N;
      int indexb = (j % ocean->N) + ((i + 1) % ocean->N) * ocean->N;

      double ht = 0.5 + (200.0 / LENGTH) * ocean->vertices[index].nz;
      double htl = 0.5 + (200.0 / LENGTH) * ocean->vertices[indexl].nz;
      double htr = 0.5 + (200.0 / LENGTH) * ocean->vertices[indexr].nz;
      double htt = 0.5 + (200.0 / LENGTH) * ocean->vertices[indext].nz;
      double htb = 0.5 + (200.0 / LENGTH) * ocean->vertices[indexb].nz;

      for (int c = 0; c < 9; c++) {
        if (map->m_height->get(i, j) * 255 < threshold[c]) {
          this->set(i, j, colors[c]);
          break;
        }

        if (map->m_height->get(i, j) * 255 >= threshold[8]) {
          this->set(i, j, 0xFFEEEEEE);
          break;
        }
      }

      // // moisture lerping
      // if (map->m_height->at(i,j)  * 255 > threshold[2] &&
      // map->m_moisture->at(i,j) < 0.1) {
      //   this->at(i,j) = lerpColor(
      //               lerpColor(0xFFEEDDBB, 0xFF5B3F31, (map->m_height->at(i,j)
      //               - 0.458) * 0.8),
      //                    this->at(i,j), 0.2 + 0.8 * map->m_moisture->at(i,j)
      //                    * 10.0);
      // }
      float wat = 0.6;

      // TODO: why can't I just assign it in one go???s
      Vec3<double> norm;
      norm = map->m_normal->at(i, j);
      norm.z *= 2;
      norm.normalize();
      // exit(0);

      double doot = m_light.dot(norm);
      // doot = 1.0;
      // printf("%f\n", m_light.z);

      if (doot < 0) {
        doot = 0;
      }

      if (m_light.z > -0.1 && m_light.z <= 0) {
        double a = 1.0 + m_light.z * 10;

        wat += wat * 0.8 * a;
      }

      if (m_light.z > 0) {
        wat *= doot;
        // wat *= doot * m_elevation;
        this->at(i, j) =
            multiplyColor(this->at(i, j), wat + 0.4, wat + 0.4, wat + 0.4);
      } else {
        double doot2 = m_moonlight.dot(map->m_normal->at(i, j));

        wat *= doot * 0.2;

        this->at(i, j) =
            multiplyColor(this->at(i, j), wat + 0.4, wat + 0.4, wat + 0.4);
      }

      // render water
      if (map->m_water->m_water_avg->at(i, j) >= 0.001) {
        float height =
            map->m_water->m_water_avg->at(i, j) + map->m_height->at(i, j);
        // this->at(i, j) = 0xFF3A5BAA;

        double alpha = 0.7 + 3 * map->m_water->m_water_avg->at(i, j);
        if (alpha > 1.0) {
          alpha = 1.0;
        }

        // 0xFF1A2B56, 0xFF253C78

        Vec3<double> wcolor(0x1A, 0x2B, 0x56);
        Vec3<double> ocolor;
        ocolor.x = (this->at(i, j) & 0x00FF0000) >> 16;
        ocolor.y = (this->at(i, j) & 0x0000FF00) >> 8;
        ocolor.z = this->at(i, j) & 0x000000FF;

        if (m_light.z > 0) {
          norm.x = htr - htl;
          norm.y = htb - htt;
          norm.z = 1.0;

          norm.x *= sqrt(map->m_water->m_water_avg->at(i, j));
          norm.y *= sqrt(map->m_water->m_water_avg->at(i, j));

          norm.normalize();

          // double doot = m_light.dot(norm) * 0.4;

          // this->at(i, j) = this->at(i,j) +
          //     multiplyColor( 0xFF3A5BAA, doot, doot, doot);

          Vec3<double> sunlight = m_light;

          // Vec3<double> eyeVec((double)j / m_cols - 0.5,
          //                     (double)i / m_rows - 0.5, 0.5);

          Vec3<double> eyeVec(0, 0, 1);

          sunlight.normalize();

          // printf("%f\n", eyeVec.x);
          eyeVec.x *= -(double)m_cols / m_rows;
          eyeVec.y *= -1;
          eyeVec.normalize();

          Vec3<double> half = eyeVec + sunlight;
          // norm.x = 0;
          // norm.y = 0;
          // norm.z = 1;

          half.normalize();
          double boop = half.dot(norm);
          boop = pow(boop, 8) * 0.5;

          this->at(i, j) = lerpColor(this->at(i, j), 0xFF3A5BAA, boop);
          // this->at(i, j) = lerpColor(this->at(i, j),  0xFFFFFFFF, boop);
          wcolor.x += (0x5A - wcolor.x) * boop;
          wcolor.y += (0x8B - wcolor.y) * boop;
          wcolor.z += (0xCA - wcolor.z) * boop;

          half.x = (double)j / m_rows - 0.5 - m_light.x * 2.0;
          half.y = (double)i / m_rows - 0.5 - m_light.y * 2.0;
          half.z = 0.5;
          half.normalize();
          boop = half.dot(norm);
          boop = pow(boop, 16) * 0.8;

          this->at(i, j) = lerpColor(this->at(i, j), 0xFFFFFFFF, boop);
          wcolor.x += (0xFF - wcolor.x) * boop;
          wcolor.y += (0xFF - wcolor.y) * boop;
          wcolor.z += (0xFF - wcolor.z) * boop;
        }

        wcolor = wcolor * alpha;
        wcolor.x = ((int)wcolor.x) & 0xFF;
        wcolor.y = ((int)wcolor.y) & 0xFF;
        wcolor.z = ((int)wcolor.z) & 0xFF;

        ocolor = ocolor * (1.0 - alpha);

        ocolor.x = ((int)ocolor.x) & 0xFF;
        ocolor.y = ((int)ocolor.y) & 0xFF;
        ocolor.z = ((int)ocolor.z) & 0xFF;

        ocolor = wcolor + ocolor;

        ocolor.x = ((int)ocolor.x) & 0xFF;
        ocolor.y = ((int)ocolor.y) & 0xFF;
        ocolor.z = ((int)ocolor.z) & 0xFF;

        this->at(i, j) = 0xFF000000 | (int)ocolor.x << 16 | (int)ocolor.y << 8 |
                         (int)ocolor.z;

        norm.x = 0 + 0.1 * sin(calcMs / 10.0 + i);
        norm.y = 0 + 0.1 * cos(calcMs / 10.0 + j);
      }

      norm.x = 0;
      norm.y = 0;
      norm.z = 0;

      // do render stuff

      if (map->m_height->at(i, j) * 255 < threshold[2]) {
        wat = 0.6;
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

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // errr how do I do this....

      if (map->m_height->at(i, j) > 0.7) {
        this->at(i, j) = c_rock;
      } else {
        if (map->m_moisture->at(i, j) > 0.05) {
          this->at(i, j) = lerpColor(c_drydirt, c_wetdirt,
                                     0.4 + map->m_moisture->at(i, j) * 0.6);
        } else {
          this->at(i, j) = c_drydirt;
        }

        if (map->m_vegetation->at(i, j) > 0.2 &&
            map->m_moisture->at(i, j) > 0.3) {
          this->at(i, j) = c_grass;
        }

        if (map->m_vegetation->at(i, j) > 0.65 &&
            map->m_moisture->at(i, j) > 0.4) {
          this->at(i, j) = c_forest;
        }

        if (map->m_vegetation->at(i, j) > 0.8 &&
            map->m_moisture->at(i, j) > 0.8) {
          this->at(i, j) = c_rainforest;
        }
      }

      // render water
      if (map->m_water->m_water_avg->at(i, j) > 0.001) {
        float height =
            map->m_water->m_water_avg->at(i, j) + map->m_height->at(i, j);
        this->at(i, j) = 0xFF305090;

        int rip = floor(height * 128.0) - 32;
        this->at(i, j) = shiftColor(this->at(i, j), rip, rip, rip);
      } else {
        float doot = m_light.dot(map->m_normal->at(i, j));
        float wat = 0;
        if (m_light.z > 0 && doot >= 0) {
          wat *= 0.6 * doot;
        } else {
          wat *= 0.4;
        }
        this->at(i, j) =
            multiplyColor(this->at(i, j), wat + 0.4, wat + 0.4, wat + 0.4);
      }
    }
  }
}

void TrColorMap::updateLightAngle() {
  Vec2<double> bleh = getSunPos(m_month, m_day, m_hour);

  m_light.z = sin(bleh[1]);
  m_light.x = cos(bleh[1]) * sin(bleh[0]);
  m_light.y = cos(bleh[1]) * cos(bleh[0]);

  m_elevation = bleh[1];

  Vec2<double> melhe = getMoonPos(m_month, m_day, m_hour);

  m_moonlight.z = sin(melhe[1]);
  m_moonlight.x = cos(melhe[1]) * sin(melhe[0]);
  m_moonlight.y = cos(melhe[1]) * cos(melhe[0]);
};