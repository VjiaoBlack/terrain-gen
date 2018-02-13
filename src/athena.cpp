/**
 * test_driving.cpp
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <set>

#include "TrMap/TrMap.hpp"
#include "TrGame.hpp"

int main(int argv, char* argc[]) {
    TrGame renderer;
    renderer.run();

    return 0;
}