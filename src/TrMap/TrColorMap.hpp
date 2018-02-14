
#pragma once

#include "TrMapData.hpp"

class TrMap;

// map
// is this too much memory? make sure to check
class TrColorMap : public TrMapData<uint32_t> {
public:
  int m_renderState;

  TrColorMap(int rows, int cols) 
  : TrMapData<uint32_t>(rows, cols)
  , m_renderState(0) {}

  ~TrColorMap() {}

  void update(TrMap* map);  

  void updateGrayscale(TrMap* map);
  void updateHistogram(TrMap* map);
  void updateDisplay(TrMap* map);
  void updateMoistureDemo(TrMap* map);
};


