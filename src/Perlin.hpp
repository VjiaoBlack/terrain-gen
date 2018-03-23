
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
  double fade(double t);
  double lerp(double t, double a, double b);
  double grad(int hash, double x, double y, double z);
};
