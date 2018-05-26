
#include "TrGame.hpp"
#include "TrECS/TrEntityTypes/TrEntityType.hpp"
#include "../TrECS/TrSystems/TrEntitySystem.hpp"
#include "TrECS/TrEntityTypes/TrPlantEntityType.hpp"

int LENGTH = 400;
TrColorMap::TrColorMap(int rows, int cols)
    : TrMapData<uint32_t>(rows, cols),
      m_renderState(0),
      ocean(new cOcean(64, 0.0005, vector2(3, 3), LENGTH, true)) {
  m_light = dvec3(0, 0, 1);
  m_light = normalize(m_light);
  m_hour = 6;
  m_day = 1;
  m_month = 6;
}
void TrColorMap::update(TrMap *map) {
  switch (map->m_renderState) {
    case 0:this->updateGrayscale(map);
      break;

    case 1:this->updateHistogram(map);
      break;

    case 2:this->updateDisplay(map);
      break;

    case 3:
    default:this->updateMoistureDemo(map);
      break;
  }
}

void TrColorMap::updateGrayscale(TrMap *map) {
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // render land
      this->at(i,j) = multiplyColor(0xFFFFFFFF, map->m_height->at(i, j),
                                    map->m_height->at(i, j),
                                    map->m_height->at(i, j));
    }
  }
}

void TrColorMap::updateHistogram(TrMap *map) {
  // buffer counts in the top row, set to 0 later.

  for (int j = 0; j < m_cols; j++) {
    this->at(0, j) = 0;
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
    double amt = (double) this->at(0, j) / (double) m_cols;
    if (floor(cur_col + amt) > floor(cur_col)) {
      if (cur_col < m_cols) {
        for (int r = j * m_rows / m_cols; r < m_rows; r++) {
          this->at(r, (int) floor(cur_col)) = 0xFFFFFFFF;
        }
      }
    }

    cur_col += amt;
  }

  for (int j = 0; j < m_cols; j++) {
    this->at(0, j) = 0xFF000000;
  }
}

void TrColorMap::updateDisplay(TrMap *map) {
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

  double deriv = 200.0 / LENGTH;

  dvec3 temp_light = m_light;

  if (m_terrace) {
    temp_light.z /= 6.0;
    temp_light = normalize(temp_light);
  }

  dvec3 temp_light_a = sphToCart(cartToSph(temp_light) + dvec3(0, 0, -0.2));
  dvec3 temp_light_b = sphToCart(cartToSph(temp_light) + dvec3(0, 0, 0.2));

  for (int i = 1; i < m_rows - 1; i++) {
    for (int j = 1; j < m_cols - 1; j++) {
      // render land

      for (int c = 0; c < 9; c++) {
        if (map->m_height->at(i, j) * 255 < threshold[c]) {
          this->at(i, j) = colors[c];
          break;
        }

        if (map->m_height->at(i, j) * 255 >= threshold[8]) {
          this->at(i, j) = 0xFFEEEEEE;
          break;
        }
      }

      double ambient = 0.4;
      double directional = 0.6;

      dvec3 norm = map->m_normal->at(i, j);

      if (m_terrace) {
        norm.z += 2.0;
        norm = normalize(norm);
      }

      double LdotN = clamp(dot(m_light, norm), 0.0, 1.0);
      double MdotN = 0;

      if (temp_light.z > -0.1 && temp_light.z <= 0) {
        directional *= lerp5(0.0, 1.0, -0.1, temp_light.z, 0.0);
      } else if (temp_light.z > 0) {
      } else {
        LdotN = 0;
        MdotN = 0.2 * dot(m_moonlight, map->m_normal->at(i, j));
      }

      // do raytracing... but not too far? Trace, let's say,
      // 256 units.

      if (m_raytrace) {
        if (m_terrace) {
          vec3 ray1(j, i, map->m_height->m_terrace.at(i, j) * 32);
          vec3 ray1_a(j, i, map->m_height->m_terrace.at(i, j) * 32);
          vec3 ray1_b(j, i, map->m_height->m_terrace.at(i, j) * 32);

          vec3 ray2(j, i, -map->m_height->m_terrace.at(i, j) * 32);
          vec3 ray2_a(j, i, -map->m_height->m_terrace.at(i, j) * 32);
          vec3 ray2_b(j, i, -map->m_height->m_terrace.at(i, j) * 32);

          for (int step = 0; step < 1; step++) {
            ray1 += temp_light;
            ray1_a += temp_light_a;
            ray1_b += temp_light_b;
            ray2 += temp_light;
            ray2_a += temp_light_a;
            ray2_b += temp_light_b;

            if (this->isOut(ray1.y, ray1.x) || this->isOut(ray2.y, ray2.x) ||
                this->isOut(ray1_a.y, ray1_a.x) ||
                this->isOut(ray2_a.y, ray2_a.x) ||
                this->isOut(ray1_b.y, ray1_b.x) ||
                this->isOut(ray2_b.y, ray2_b.x)) {
              break;
            }

            if (map->m_height->m_terrace.bilerp(ray1.y, ray1.x) * 32 > ray1.z ||
                map->m_height->m_terrace.bilerp(ray1_a.y, ray1_a.x) * 32 >
                    ray1_a.z ||
                map->m_height->m_terrace.bilerp(ray1_b.y, ray1_b.x) * 32 >
                    ray1_b.z) {
              LdotN -= 0.25;
              break;
            } else if (-map->m_height->m_terrace.bilerp(ray2.y, ray2.x) * 32 >
                ray2.z ||
                -map->m_height->m_terrace.bilerp(ray2_a.y, ray2_a.x) *
                    32 >
                    ray2_a.z ||
                -map->m_height->m_terrace.bilerp(ray2_b.y, ray2_b.x) *
                    32 >
                    ray2_b.z) {
              LdotN += 0.3;
              break;
            }
          }

          vec3 ray3(j, i, map->m_height->m_terrace.at(i, j) * 32);

          for (int step = 0; step < 2; step += 1) {
            ray3 += temp_light * 1.0;
            if (map->m_height->m_terrace.bilerp(ray3.y, ray3.x) * 32 > ray3.z) {
              LdotN -= 0.25;
              break;
            }
          }
        } else {
          vec3 ray(j, i, map->m_height->at(i, j) * 256);

          for (int step = 0; step < 32; step += 1) {
            ray += m_light * 1.0;
            if (map->m_height->bilerp(ray.y, ray.x) * 256 > ray.z) {
              LdotN = 0.0;
              break;
            }
          }
        }
      }

      directional *= clamp(LdotN + MdotN, -ambient, 2.0);

      this->at(i, j) =
          multiplyColor(this->at(i, j), directional + ambient,
                        directional + ambient, directional + ambient);

      // render water
      if (map->m_water->m_water_avg->at(i, j) >= 0.001) {
        float height =
            map->m_water->m_water_avg->at(i, j) + map->m_height->at(i, j);

        double alpha =
            lerp5(0.6, 1.0, 0.001, map->m_water->m_water_avg->at(i, j), 0.10);
        // alpha = clamp(0.6, alpha, 1.0);
        alpha = clamp(alpha, 0.6, 1.0);

        dvec3 ocolor = colorToVec(this->at(i, j));
        dvec3 wcolor = m_deepWater;

        if (m_light.z > 0) {
          // construct fake "normal" of the water surface
          norm.x =
              deriv * ocean->vertices->gaussDx(i % ocean->N, j % ocean->N).nz;
          norm.y =
              deriv * ocean->vertices->gaussDy(i % ocean->N, j % ocean->N).nz;
          norm.z = 1.0;

          // modulate wave intensity by water depth
          norm.x *= sqrt(map->m_water->m_water_avg->at(i, j));
          norm.y *= sqrt(map->m_water->m_water_avg->at(i, j));

          norm = normalize(norm);

          // using orthographic (0, 0, 1) eye vector
          dvec3 sunlight = normalize(m_light);
          dvec3 half = normalize(sunlight + dvec3(0, 0, 1));

          double HdotN = dot(half, norm);
          HdotN = pow(HdotN, 8) * 0.5;

          this->at(i, j) =
              lerpColor(this->at(i, j), vecToColor(m_shallowWater), HdotN);
          wcolor += (m_mediumWater - wcolor) * HdotN;

          half.x = (double) j / m_rows - 0.5 - m_light.x * 2.0;
          half.y = (double) i / m_rows - 0.5 - m_light.y * 2.0;
          half.z = 0.5;
          half = normalize(half);
          HdotN = dot(half, norm);
          HdotN = pow(HdotN, 16) * 0.8;

          this->at(i, j) = lerpColor(this->at(i, j), 0xFFFFFFFF, HdotN);
          wcolor += (dvec3(0xFF) - wcolor) * HdotN;
        }

        wcolor = clamp(wcolor * alpha, 0.0, 256.0);
        ocolor = clamp(ocolor * (1.0 - alpha), 0.0, 256.0);
        ocolor = clamp(wcolor + ocolor, 0.0, 256.0);

        this->at(i, j) = vecToColor(ocolor);

      }
    }
  }
}

void TrColorMap::updateMoistureDemo(TrMap *map) {
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
        float LdotN = dot(m_light, map->m_normal->at(i, j));
        float directional = 0;
        if (m_light.z > 0 && LdotN >= 0) {
          directional *= 0.6 * LdotN;
        } else {
          directional *= 0.4;
        }
        this->at(i, j) = multiplyColor(this->at(i, j), directional + 0.4,
                                       directional + 0.4, directional + 0.4);
      }
    }
  }
}

void TrColorMap::updateLightAngle() {
  dvec2 bleh = getSunPos(m_month, m_day, m_hour);

  m_light.z = sin(bleh[1]);
  m_light.x = cos(bleh[1]) * sin(bleh[0]);
  m_light.y = cos(bleh[1]) * cos(bleh[0]);

  m_elevation = bleh[1];

  dvec2 melhe = getMoonPos(m_month, m_day, m_hour);

  m_moonlight.z = sin(melhe[1]);
  m_moonlight.x = cos(melhe[1]) * sin(melhe[0]);
  m_moonlight.y = cos(melhe[1]) * cos(melhe[0]);
};