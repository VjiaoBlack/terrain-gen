#pragma once

#include <set>
#include <unordered_map>
#include <unordered_set>
#include "../Utils.hpp"
#include "TrMapData.hpp"

using namespace std;

class TrMap;

class TrWaterPoint {
 public:
  double _id;
  mutable double x, y;
  mutable double amt;
  mutable double vx, vy;
  mutable double dirt;

  TrWaterPoint() : TrWaterPoint(-1.0, -1.0, -1.0) { amt = 0; }
  TrWaterPoint(double id, double tx, double ty)
      : _id(id), x(tx), y(ty), amt(0.001), vx(0), vy(0), dirt(0) {}

  bool operator<(const TrWaterPoint &right) const { return _id < right._id; }

  bool operator==(const TrWaterPoint &right) const { return _id == right._id; }

  TrWaterPoint &operator=(const TrWaterPoint &right) = default;

  static TrWaterPoint Merge(const TrWaterPoint &a, const TrWaterPoint &b) {
    if (a.amt < 0.00001 && b.amt < 0.00001) {
      // printf("    neone\n");
      return TrWaterPoint();
    } else if (a.amt < 0.00001) {
      // printf("    b\n");
      return b;
    } else if (b.amt < 0.00001) {
      // printf("    a\n");
      return a;
    } else {
      double a_to_b = b.amt / (a.amt + b.amt);

      TrWaterPoint w = a;
      w.x = lerp3(a.x, b.x, a_to_b);
      w.y = lerp3(a.y, b.y, a_to_b);
      w.vx = lerp3(a.vx, b.vx, a_to_b);
      w.vy = lerp3(a.vy, b.vy, a_to_b);
      w.amt += b.amt;
      w.dirt += b.dirt;

      // if (w.x < 0 || w.y < 0) {
      //   printf("    %f, %f | %f\n",  w.x, w.y, a_to_b);
      //   printf("    x: %f, %f\n", a.x, b.x);
      //   printf("    y: %f, %f\n", a.y, b.y);
      //   printf("\n");
      // }

      return w;
    }
  }
};

namespace std {
template<>
struct hash<TrWaterPoint> {
  size_t operator()(const TrWaterPoint &obj) const {
    return hash<double>()(obj._id);
  }
};
}

class TrWaterTempMap : public TrMapData<double> {
 public:
  TrWaterTempMap(int rows, int cols) : TrMapData(rows, cols) {}

  void update(TrMap *map) override {};
};

class TrWaterDataMap : public TrMapData<TrWaterPoint> {
 public:
  TrWaterDataMap(int rows, int cols) : TrMapData(rows, cols) {}

  void update(TrMap *map) override {}
};

// map
// is this too much memory? make sure to check
class TrWaterMap : public TrMapData<double> {
 public:
  std::unordered_map<double, TrWaterPoint> m_water;

  //
  // TrWaterDataMap* m_water_temp;
  TrWaterTempMap *m_water_temp;
  // TrWaterDataMap* m_water_data;
  TrWaterTempMap *m_water_avg;

  TrWaterMap(int rows, int cols)
      : TrMapData(rows, cols),
        m_water_temp(new TrWaterTempMap(rows, cols)),
        m_water_avg(new TrWaterTempMap(rows, cols)) {
    // , m_water_temp(new TrWaterDataMap(rows, cols)) {
  }

  ~TrWaterMap() override {
    delete m_water_temp;
    delete m_water_avg;
  }

  void update(TrMap *map) override;
  void experimentalUpdate(TrMap *map);

  void rain(TrMap *map);
  void experimentalRain(TrMap *map);
};
