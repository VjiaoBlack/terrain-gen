#pragma once

 /**
 * TrGraphics.hpp
 *
 * Victor Jiao
 *
 * Stores wrappers for SDL2
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <random>
#include <set>

#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <random>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <math.h>

class TrMap;


#include "TrColorMap.hpp"
#include "TrHeightMap.hpp"
#include "TrMoistureMap.hpp"
#include "TrNormalMap.hpp"
#include "TrVegetationMap.hpp"
#include "TrWaterMap.hpp"
#include "TrWindMap.hpp"

using namespace std;


 
// map
// is this too much memory? make sure to check
class TrMap {
public:
    int m_rows, m_cols;

    TrColorMap* m_color;
    TrHeightMap* m_height;
    TrMoistureMap* m_moisture;
    TrNormalMap* m_normal;
    TrVegetationMap* m_vegetation;
    TrWaterMap* m_water;
    TrWindMap* m_wind;

    int m_renderState; 
    int m_erosionState;
    
    std::random_device m_xrandDevice;
    std::default_random_engine m_xrandEngine;
    std::uniform_int_distribution<int> m_xrandDist;
    std::uniform_real_distribution<double> m_xfrandDist;

    std::random_device m_yrandDevice;
    std::default_random_engine m_yrandEngine;
    std::uniform_int_distribution<int> m_yrandDist;
    std::uniform_real_distribution<double> m_yfrandDist;
    
    TrMap(int rows, int cols);


    ~TrMap() {
        delete m_color;
        delete m_height;
        delete m_moisture;
        delete m_normal;
        delete m_vegetation;
        delete m_water;
        delete m_wind;
    }

    
    void update(set<int> keysDown);
    void printSimpleHeightMatrix();
    void saveMap();

};

