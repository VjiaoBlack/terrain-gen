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
#include <unordered_set>

#include "TrGraphics.hpp"
#include "TrRenderer.hpp"

int main(int argv, char* argc[]) {
    TrRenderer renderer;
    renderer.run();

    return 0;
}