/**
* TrGraphics
*
* Victor Jiao
*
* a test file for testing driving.
*/

#include "TrMap.hpp"
#include "../TrGame.hpp"
#include "TrECS/TrSystems/TrEntitySystem.hpp"
#include "TrECS/MyEntities.hpp"

TrMap::TrMap(int rows, int cols, TrGame* game)
    : m_rows(rows),
      m_cols(cols),
      m_game(game),
      m_renderState(2),  // TODO: default render state is 2
      m_erosionState(0),
      m_color(new TrColorMap(rows, cols)),
      m_height(new TrHeightMap(rows, cols)),
      m_moisture(new TrMoistureMap(rows, cols)),
      m_normal(new TrNormalMap(rows, cols)),
      m_vegetation(new TrVegetationMap(rows, cols)),
      m_water(new TrWaterMap(rows, cols)),
      m_wind(new TrWindMap(rows, cols)),
      m_entityHeight(new TrMapData<double>(rows, cols)),
      m_entityColor(new TrMapData<uint32_t>(rows, cols)),
      m_entityNormal(new TrMapData<dvec3>(rows, cols)) {
  m_xrandEngine = std::default_random_engine(m_xrandDevice());
  m_yrandEngine = std::default_random_engine(m_yrandDevice());

  // TODO fix _X _Y
  m_xrandDist = std::uniform_int_distribution<int>(0, K_MAP_SIZE_X);
  m_xfrandDist = std::uniform_real_distribution<double>(0, K_MAP_SIZE_X);

  m_yrandDist = std::uniform_int_distribution<int>(0, K_MAP_SIZE_Y);
  m_yfrandDist = std::uniform_real_distribution<double>(0, K_MAP_SIZE_Y);

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      m_normal->at(i, j).z = 1.0f;
      m_wind->at(i, j).y = 1.0f;
    }
  }

  m_height->perlinNoise(K_MAP_SIZE_Y, 8, 1.5, 1.0);

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      m_height->at(i, j) -= 1.0;
      m_height->at(i, j) *= 1.2;

      m_height->at(i, j) =
          m_height->at(i, j) * pow(fabs(m_height->at(i, j)), 0.8);

      m_height->at(i, j) += 0.5;

      if (m_height->at(i, j) > 1.0) {
        m_height->at(i, j) = 1.0;
      } else if (m_height->at(i, j) < 0.0) {
        m_height->at(i, j) = 0.0;
      }
    }
  }

  m_height->update(this);

  float water_height = 0.48;
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      if (m_height->at(i, j) <= water_height) {
        m_water->at(i, j) = water_height - m_height->at(i, j) + 0.001;
      }
    }
  }

  m_normal->update(this);
  m_color->update(this);
}

TrMap::~TrMap() = default;

void TrMap::update(set<int> keysDown) {
  int m_speed = 1;
  for (auto key : keysDown) {
    switch (key) {
      case SDLK_u:
        for (int i = 0; i < K_MAP_SIZE_Y; i++) {
          for (int j = 0; j < K_MAP_SIZE_X; j++) {
            this->m_height->m_data[i * K_MAP_SIZE_X + j] -=
                (double) m_speed / 255.0;
          }
        }
        m_toUpdate.insert(m_color);

        break;
      case SDLK_i:
        for (int i = 0; i < K_MAP_SIZE_Y; i++) {
          for (int j = 0; j < K_MAP_SIZE_X; j++) {
            this->m_height->m_data[i * K_MAP_SIZE_X + j] +=
                (double) m_speed / 255.0;
          }
        }
        m_toUpdate.insert(m_color);

        break;
      case SDLK_b:
        for (int i = 0; i < m_speed; i++) {
          this->m_height->boxBlur();
        }
        m_toUpdate.insert(m_color);

        break;
      case SDLK_r:m_water->rain(this);
        m_toUpdate.insert(m_color);
        break;
      case SDLK_d:this->m_water->set(0.0);
        m_toUpdate.insert(m_color);
        break;
      case SDLK_e:this->m_erosionState = !this->m_erosionState;
        usleep(100000);
        break;

      case SDLK_w:
        // animate water engine
        for (int i = 0; i < m_speed; i++) {
          m_water->update(this);
        }
        m_toUpdate.insert(m_color);
        break;
      case SDLK_n:
        // reclaculate normals
        m_toUpdate.insert(m_height);
        m_toUpdate.insert(m_normal);
        m_toUpdate.insert(m_color);
        break;
      case SDLK_m:
        // update water!! omg
        for (int i = 0; i < m_speed; i++) {
          m_moisture->update(this);
        }
        m_toUpdate.insert(m_color);
        break;
      case SDLK_k:
        // toggle moisture / not
        m_renderState++;
        if (m_renderState > 3) {
          m_renderState = 0;
        }

        m_toUpdate.insert(m_color);
        usleep(100000);

        break;
      case SDLK_j:m_wind->update(this);
        break;
      case SDLK_s:
        // Save the map: color and heightmap
        this->saveMap();
        break;
      case SDLK_t:m_color->m_hour += 0.06;
        if (m_color->m_hour > 24) {
          m_color->m_hour -= 24;
          m_color->m_day += 1;
        }
        if (m_color->m_month > 12) {
          m_color->m_month -= 12;
        }

        m_color->updateLightAngle();
        m_toUpdate.insert(m_color);
        break;
      case SDLK_y:m_color->m_raytrace = !m_color->m_raytrace;
        m_toUpdate.insert(m_color);
        break;
      case SDLK_h:m_color->m_terrace++;
        if (m_color->m_terrace > 3) {
          m_color->m_terrace = 0;
        }
        m_toUpdate.insert(m_color);
        break;
      default:break;
    }
  }

  // draw plants into entity color buffer
  for (auto plant : m_game->m_entSystem->m_plants) {
    plant->update(m_game);

    // draw plants into entity height buffer
    for (int r = plant->m_rect.y; r < plant->m_rect.y + plant->m_rect.h; r++) {
      for (int c = plant->m_rect.x; c < plant->m_rect.x + plant->m_rect.w; c++) {
        m_entityHeight->at(r, c) = 0.03125;
      }
    }
  }

  // update entity normal buffer
  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // l->r
      dvec3 lr(2.0, 0.0, 255.0 * m_entityHeight->sample<TrGaussDx>(i, j));
      // u->d
      dvec3 ud(0.0, 2.0, 255.0 * m_entityHeight->sample<TrGaussDy>(i, j));

      m_entityNormal->at(i, j) = cross(lr, ud);
      m_entityNormal->at(i, j) = normalize(m_entityNormal->at(i, j));
    }
  }

  for (auto mapdata : m_toUpdate) {
    mapdata->update(this);
  }
}

void TrMap::printSimpleHeightMatrix() {
  int threshold[9] = {70, 100, 117, 132, 148, 165, 180, 198, 218};

  uint32_t simpleHeights[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

  std::cout << m_rows << " " << m_cols << endl;

  for (int i = 0; i < m_rows; i++) {
    for (int j = 0; j < m_cols; j++) {
      // render land
      for (int c = 0; c < 9; c++) {
        if (m_height->at(i, j) * 255 < threshold[c]) {
          std::cout << simpleHeights[c] << " ";
          break;
        }

        if (m_height->at(i, j) * 255 >= threshold[8]) {
          std::cout << 9 << " ";
          break;
        }
      }
    }
    std::cout << endl;
  }
}

void TrMap::saveMap() {
  SDL_Surface *surface, *surface2;

  surface = SDL_CreateRGBSurface(0, K_MAP_SIZE_X, K_MAP_SIZE_Y, K_RGBA_BYTES,
                                 K_R_MASK, K_G_MASK, K_B_MASK, K_A_MASK);

  SDL_LockSurface(surface);

  // memcpy(surface->pixels, this->m_color->m_data, K_MAP_SIZE * K_MAP_SIZE *
  // 4);

  SDL_UnlockSurface(surface);

  IMG_SavePNG(surface, "color.png");
  SDL_FreeSurface(surface);

  TrColorMap map_height(K_MAP_SIZE_X, K_MAP_SIZE_Y);

  for (int i = 0; i < K_MAP_SIZE_Y; i++) {
    for (int j = 0; j < K_MAP_SIZE_X; j++) {
      // map_height.at(i,j) = floor(this->m_height->at(i,j) * 255);
    }
  }

  for (int i = 0; i < K_MAP_SIZE_Y; i++) {
    for (int j = 0; j < K_MAP_SIZE_X; j++) {
      map_height.m_data[i * K_MAP_SIZE_X + j] |=
          map_height.m_data[i * K_MAP_SIZE_X + j] << 8u;
      map_height.m_data[i * K_MAP_SIZE_X + j] |=
          map_height.m_data[i * K_MAP_SIZE_X + j] << 16u;
      map_height.m_data[i * K_MAP_SIZE_X + j] |=
          map_height.m_data[i * K_MAP_SIZE_X + j] << 24u;
      map_height.m_data[i * K_MAP_SIZE_X + j] =
          map_height.m_data[i * K_MAP_SIZE_X + j] | 0xFF000000;
    }
  }

  surface2 = SDL_CreateRGBSurface(0, K_MAP_SIZE_X, K_MAP_SIZE_Y, K_RGBA_BYTES,
                                  K_R_MASK, K_G_MASK, K_B_MASK, K_A_MASK);

  SDL_LockSurface(surface2);
  memcpy(surface2->pixels, map_height.m_data,
         K_MAP_SIZE_X * K_MAP_SIZE_Y * sizeof(uint32_t));
  SDL_UnlockSurface(surface2);

  IMG_SavePNG(surface2, "height.png");

  SDL_FreeSurface(surface2);
}