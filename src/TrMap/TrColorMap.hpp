
#pragma once

#include "../Perlin.hpp"
#include "TrMapData.hpp"
#include "../../include/fft/fft.h"

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
  Vec3<double> m_light;
  Vec3<double> m_moonlight;
  double m_hour;
  int m_day;
  int m_month;
  double m_elevation;

  cOcean* ocean;

  PerlinNoise* m_perlinx;
  PerlinNoise* m_perliny;

  TrColorMap(int rows, int cols);

  ~TrColorMap() {
    delete m_perlinx;
    delete ocean;
    delete m_perliny;
  }

  void update(TrMap* map);

  void updateLightAngle();

  void updateGrayscale(TrMap* map);
  void updateHistogram(TrMap* map);
  void updateDisplay(TrMap* map);
  void updateMoistureDemo(TrMap* map);
};
