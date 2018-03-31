#pragma once

/**
 * TrCSVObject.hpp
 *
 * Basically wraps fast-cpp-csv-parser and imports the data from .csv filesls
 * Note: all filepaths are from point of view of root directory of the project
 */
#include <iostream>
#include <map>
#include <string>

#include "../../include/fast-cpp-csv-parser/csv.h"

#include "../TrECS/TrEntities.hpp"
#include "../TrECS/TrBuildingEntity.hpp"
using namespace std;

class TrData {
 private:
  TrData();

  map<string, TrEntity*> m_entities;
  // map<string, TrEntity*> m_entities;
  // map<string, TrEntity*> m_entities;

 public:
  static TrData& getInstance();
};