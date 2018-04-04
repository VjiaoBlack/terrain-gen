
#pragma once
/**
 * Perlin.hpp
 *
 * This is not my code
 *
 * Please check out this link
 *
 * https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
 *
 */

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <vector>

#include "Utils.hpp"

// Perlin Noise
class PerlinNoise {
  // The permutation vector
  std::vector<int> p;

 public:
  // Initialize with the reference values for the permutation vector
  PerlinNoise();
  // Generate a new permutation vector based on the value of seed
  PerlinNoise(unsigned int seed);
  // Get a noise value, for 2D images z can have any value
  double noise(double x, double y, double z);

 private:
  inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); };
  inline double lerp(double t, double a, double b) { return a + t * (b - a); };

  inline double grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    // Convert lower 4 bits of hash into 12 gradient directions
    double u = h < 8 ? x : y, v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
  };
};
