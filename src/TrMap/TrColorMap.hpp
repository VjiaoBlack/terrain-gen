
#pragma once

#include "../../include/fft/fft.h"
#include "../Perlin.hpp"
#include "../TrUtils/TrMath.hpp"
#include "../TrUtils/TrSimulation.hpp"
#include "TrMapData.hpp"

class TrMap;

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

  TrColorMap(int rows, int cols);

  ~TrColorMap() { delete ocean; }

  void update(TrMap* map);

  void updateLightAngle();

  void updateGrayscale(TrMap* map);
  void updateHistogram(TrMap* map);
  void updateDisplay(TrMap* map);
  void updateMoistureDemo(TrMap* map);
};
