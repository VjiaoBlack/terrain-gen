#pragma once

/**
 * TrSimulation.hpp
 */
#include "../Utils.hpp"

extern double daysAtMonth[];

// from:
// https://stackoverflow.com/questions/8708048/position-of-the-sun-given-time-of-day-latitude-and-longitude
Vec2<double> getSunPos(int month, int day, double hour);
Vec2<double> getMoonPos(int month, int day, double hour);
