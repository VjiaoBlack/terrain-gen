/**
* TrGraphics
*
* Victor Jiao
*
* a test file for testing driving.
*/

#include "TrMap.hpp"

TrMap::TrMap(int rows, int cols)
    : m_rows(rows),
      m_cols(cols),
      m_renderState(2),  // TODO: default render state is 2
      m_erosionState(0),
      m_color(new TrColorMap(rows, cols)),
      m_height(new TrHeightMap(rows, cols)),
      m_moisture(new TrMoistureMap(rows, cols)),
      m_normal(new TrNormalMap(rows, cols)),
      m_vegetation(new TrVegetationMap(rows, cols)),
      m_water(new TrWaterMap(rows, cols)),
      m_wind(new TrWindMap(rows, cols)) {
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
      m_height->at(i, j) -= 0.5;
      m_height->at(i, j) -= 0.5;
      m_height->at(i, j) *= 1.5;

      m_height->at(i, j) =
          m_height->at(i, j) * pow(fabs(m_height->at(i, j)), 0.6);

      m_height->at(i, j) += 0.5;

      if (m_height->at(i, j) > 1.0) {
        m_height->at(i, j) = 1.0;
      } else if (m_height->at(i, j) < 0.0) {
        m_height->at(i, j) = 0.0;
      }
    }
  }

  m_color->update(this);
}

void TrMap::update(set<int> keysDown) {
  int m_speed = 1;
  for (auto key : keysDown) {
    switch (key) {
      case SDLK_u:
        for (int i = 0; i < K_MAP_SIZE_Y; i++) {
          for (int j = 0; j < K_MAP_SIZE_X; j++) {
            this->m_height->m_data[i * K_MAP_SIZE_X + j] -=
                (double)m_speed / 255.0;
          }
        }
        m_color->update(this);

        break;
      case SDLK_i:
        for (int i = 0; i < K_MAP_SIZE_Y; i++) {
          for (int j = 0; j < K_MAP_SIZE_X; j++) {
            this->m_height->m_data[i * K_MAP_SIZE_X + j] +=
                (double)m_speed / 255.0;
          }
        }
        m_color->update(this);

        break;
      case SDLK_b:
        for (int i = 0; i < m_speed; i++) {
          this->m_height->boxBlur();
        }
        m_color->update(this);

        break;
      case SDLK_r:
        m_water->rain(this);
        m_color->update(this);
        break;
      case SDLK_d:
        this->m_water->set(0.0);
        m_color->update(this);
        break;
      case SDLK_e:
        this->m_erosionState = !this->m_erosionState;
        usleep(100000);
        break;

      case SDLK_w:
        // animate water engine
        for (int i = 0; i < m_speed; i++) {
          m_water->update(this);
        }
        m_color->update(this);
        break;
      case SDLK_n:
        // reclaculate normals
        m_height->update(this);
        m_normal->update(this);
        m_color->update(this);
        break;
      case SDLK_m:
        // update water!! omg
        for (int i = 0; i < m_speed; i++) {
          m_moisture->update(this);
          m_color->update(this);
        }
        break;
      case SDLK_k:
        // toggle moisture / not
        m_renderState++;
        if (m_renderState > 3) {
          m_renderState = 0;
        }

        m_color->update(this);
        usleep(100000);

        break;
      case SDLK_j:
        m_wind->update(this);
        break;
      case SDLK_s:
        // Save the map: color and heightmap
        this->saveMap();
        break;
    }
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
        if (m_height->get(i, j) * 255 < threshold[c]) {
          std::cout << simpleHeights[c] << " ";
          break;
        }

        if (m_height->get(i, j) * 255 >= threshold[8]) {
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
          map_height.m_data[i * K_MAP_SIZE_X + j] << 8;
      map_height.m_data[i * K_MAP_SIZE_X + j] |=
          map_height.m_data[i * K_MAP_SIZE_X + j] << 16;
      map_height.m_data[i * K_MAP_SIZE_X + j] |=
          map_height.m_data[i * K_MAP_SIZE_X + j] << 24;
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