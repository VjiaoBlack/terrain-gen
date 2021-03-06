/**
 * This is not my code
 *
 * Check Perlin.hpp for more deets
 */

#include "Perlin.hpp"

// Generate a new permutation vector based on the value of seed
PerlinNoise::PerlinNoise(unsigned int seed) {
  p.resize(512);

  // Fill p with values from 0 to 255
  std::iota(p.begin(), p.end(), 0);

  // Initialize a random engine with seed
  std::default_random_engine engine(seed);

  // Shuffle using the above random engine
  std::shuffle(p.begin(), p.end(), engine);

  // Duplicate the permutation vector
  p.insert(p.end(), p.begin(), p.end());
}

PerlinNoise::PerlinNoise() {
  p.resize(512);  // TODO

  // Fill p with values from 0 to 255
  std::iota(p.begin(), p.end(), 0);

  // Initialize a random engine
  std::random_device rd;
  std::default_random_engine engine(rd());

  // Suffle  using the above random engine
  std::shuffle(p.begin(), p.end(), engine);

  // Duplicate the permutation vector
  p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
  // Find the unit cube that contains the point
  int X = (uint) floor(x) & 255u;
  int Y = (uint) floor(y) & 255u;
  int Z = (uint) floor(z) & 255u;

  // Find relative x, y,z of point in cube
  x -= floor(x);
  y -= floor(y);
  z -= floor(z);

  // Compute fade curves for each of x, y, z
  double u = fade(x);
  double v = fade(y);
  double w = fade(z);

  // Hash coordinates of the 8 cube corners
  int A = p[X] + Y;
  int AA = p[A] + Z;
  int AB = p[A + 1] + Z;
  int B = p[X + 1] + Y;
  int BA = p[B] + Z;
  int BB = p[B + 1] + Z;

  // Add blended results from 8 corners of cube
  double res = lerp(
      w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x - 1, y, z)),
              lerp(u, grad(p[AB], x, y - 1, z), grad(p[BB], x - 1, y - 1, z))),
      lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
                   grad(p[BA + 1], x - 1, y, z - 1)),
           lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                grad(p[BB + 1], x - 1, y - 1, z - 1))));
  return (res + 1.0) / 2.0;
}
