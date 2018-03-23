#pragma once

#include "TrMapData.hpp"

class TrMap;

// map
// is this too much memory? make sure to check
class TrNormalMap : public TrMapData<Vec3> {
 public:
  TrNormalMap(int rows, int cols) : TrMapData<Vec3>(rows, cols) {}

  void update(TrMap* map);
};
