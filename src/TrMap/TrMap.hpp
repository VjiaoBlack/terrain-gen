#pragma once

/**
* TrGraphics.hpp
*
* Victor Jiao
*
* Stores wrappers for SDL2
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <set>
#include <vector>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <iostream>
#include <random>
#include <vector>
#include <set>

class TrMap;

#include "TrColorMap.hpp"
#include "TrHeightMap.hpp"
#include "TrMoistureMap.hpp"
#include "TrNormalMap.hpp"
#include "TrVegetationMap.hpp"
#include "TrWaterMap.hpp"
#include "TrWindMap.hpp"

#include "TrMapData.hpp"
#include <memory>

class TrGame;

// map
// is this too much memory? make sure to check
class TrMap {
 private:
  set<TrMapUpdatable*> m_toUpdate;
 public:
  int m_rows, m_cols;

  TrGame* m_game;

  std::unique_ptr<TrColorMap> m_color;
  std::unique_ptr<TrHeightMap> m_height;
  std::unique_ptr<TrMoistureMap> m_moisture;
  std::unique_ptr<TrNormalMap> m_normal;
  std::unique_ptr<TrVegetationMap> m_vegetation;
  std::unique_ptr<TrWaterMap> m_water;
  std::unique_ptr<TrWindMap> m_wind;

  std::unique_ptr<TrMapData<double>> m_entityHeight;
  std::unique_ptr<TrMapData<uint32_t>> m_entityColor;
  std::unique_ptr<TrMapData<dvec3>> m_entityNormal;

  int m_renderState;
  int m_erosionState;

  std::random_device m_xrandDevice;
  std::default_random_engine m_xrandEngine;
  std::uniform_int_distribution<int> m_xrandDist;
  std::uniform_real_distribution<double> m_xfrandDist;

  std::random_device m_yrandDevice;
  std::default_random_engine m_yrandEngine;
  std::uniform_int_distribution<int> m_yrandDist;
  std::uniform_real_distribution<double> m_yfrandDist;

  TrMap(int rows, int cols, TrGame* game);

  ~TrMap();

  void update(set<int> keysDown);
  void printSimpleHeightMatrix();
  void saveMap();
};
