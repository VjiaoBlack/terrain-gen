#pragma once

/**
 * TrSimulation.hpp
 */
#include <glm/glm.hpp>
#include "../Utils.hpp"

using namespace glm;

extern double daysAtMonth[];

// from:
// https://stackoverflow.com/questions/8708048/position-of-the-sun-given-time-of-day-latitude-and-longitude
dvec2 getSunPos(int month, int day, double hour);
dvec2 getMoonPos(int month, int day, double hour);
