/**
 * test_driving.cpp
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include <set>

#include "TrGame.hpp"
#include "TrMap/TrMap.hpp"

int main(int argv, char* argc[]) {
  TrGame renderer;
  renderer.run();

  return 0;
}