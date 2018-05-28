#pragma once
/** 
 * TrMapDataUtils.hpp
 *
 * Victor Jiao
 *
 * Includes helper functions for TrMapData objects.
 */

template<class T>
class TrMapData;

// sums array
// T and N can both be implied
template<class T, int N>
constexpr T _arr_sum(T (&array) [N]) {
  auto acc = 0.0;
  for (int i = 0; i < N; i++) {
    acc += array[i];
  }
  return acc;
}

struct TrGauss {
  static const int size = 3;
  static constexpr double data[size * size] =
      { 0.0625, 0.125 , 0.0625,
        0.125 , 0.25  , 0.125 ,
        0.0625, 0.125 , 0.0625 };
  static_assert(_arr_sum(data) == 1.0, "TrGauss kernel must sum to 1.0");
};

struct TrGaussDx {
  static const int size = 3;
  static constexpr double data[size * size] =
      {-0.25 , 0 , 0.25,
       -0.5  , 0 , 0.5   ,
       -0.25 , 0 , 0.25 };
  static_assert(_arr_sum(data) == 0.0, "TrGaussDx kernel must sum to 1.0");
};
struct TrGaussDy {
  static const int size = 3;
  static constexpr double data[size * size] =
      {-0.25 ,-0.5 ,-0.25,
        0.0  , 0.0 , 0.0 ,
        0.25 , 0.5 , 0.25 };
  static_assert(_arr_sum(data) == 0.0, "TrGaussDy kernel must sum to 1.0");
};
