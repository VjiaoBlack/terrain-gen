
#pragma once

#include "TrMapData.hpp"
#include "../Perlin.hpp"

class TrMap;

extern double daysAtMonth[];

#define TR_ASSIGN_MOD(x, y) \
  {                         \
    while (x >= y) {        \
      x -= y;               \
    }                       \
  }

// TODO: check this
#define TR_CART_TO_SPH(a, e, r, x, y, z) \
  {                                      \
    r = sqrt(x * x + y * y + z * z);     \
    e = atan2(z, sqrt(x * x + y * y));   \
    a = atan2(x, y);                     \
  }

#define TR_SPH_TO_CART(a, e, r, x, y, z) \
  {                                      \
    z = r * sin(e);                      \
    x = cos(e) * sin(a);                 \
    y = cos(e) * cos(a);                 \
  }

// map
// is this too much memory? make sure to check
class TrColorMap : public TrMapData<uint32_t> {
 public:
  int m_renderState;
  Vec3 m_light;
  Vec3 m_moonlight;
  double m_hour;
  int m_day;
  int m_month;
  double m_elevation;

  PerlinNoise* m_perlinx;
  PerlinNoise* m_perliny;

  TrColorMap(int rows, int cols)
      : TrMapData<uint32_t>(rows, cols), m_renderState(0) {
    m_light = Vec3(0, 0, 1);
    m_light.normalize();
    m_hour = 6;
    m_day = 1;
    m_month = 6;
    m_perlinx = new PerlinNoise(0);
    m_perliny = new PerlinNoise(1);

    printf("make\n");
  }

  ~TrColorMap() {
    delete m_perlinx;
    delete m_perliny;
  }

  void update(TrMap* map);

  void updateLightAngle();

  void updateGrayscale(TrMap* map);
  void updateHistogram(TrMap* map);
  void updateDisplay(TrMap* map);
  void updateMoistureDemo(TrMap* map);
};
