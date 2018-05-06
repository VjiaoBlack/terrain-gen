#pragma once

#include "TrMapData.hpp"

class TrMap;

// map
// is this too much memory? make sure to check
class TrWindMap : public TrMapData<dvec3> {
 public:
  TrWindMap(int rows, int cols) : TrMapData(rows, cols) {}

  void update(TrMap* map) override;
};
