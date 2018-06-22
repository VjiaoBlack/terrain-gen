#pragma once

/**
 * TrData.hpp
 */

#ifndef _TR_DATA_HPP_
#define _TR_DATA_HPP_

#include <iostream>
#include <map>
#include <string>
//#include <TrECS/TrEntityFactory.hpp>

#include "../../include/fast-cpp-csv-parser/csv.h"

#include "../TrECS/TrFormulas/TrFormula.hpp"
#include "../TrECS/TrItems/TrItem.hpp"

#include "../TrECS/TrEntityTypes.hpp"
using namespace std;

// TODO: figure out a way to avoid having to repeat a m_entityTypes map for each
//       entity type
static class TrData {
 public:
  static void loadData();
  static void deleteData();

  static map<string, TrActorEntityType *> m_actorTypes;
  static map<string, TrBuildingEntityType *> m_buildingTypes;
  static map<string, TrPlantEntityType *> m_plantTypes;

  static map<string, TrFormula *> m_formulas;
  static map<string, TrItem *> m_items;
};

#endif