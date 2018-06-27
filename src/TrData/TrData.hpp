#pragma once

/**
 * TrData.hpp
 */

#ifndef _TR_DATA_HPP_
#define _TR_DATA_HPP_

#include <iostream>
#include <map>
#include <string>

#include "../../include/fast-cpp-csv-parser/csv.h"

#include "../TrECS/TrFormulas/TrFormula.hpp"
#include "../TrECS/TrItems/TrItem.hpp"

#include "../TrECS/TrEntityTypes.hpp"

// TODO: figure out a way to avoid having to repeat a m_entityTypes map for each
//       entity type
class TrData {
 public:
  static void loadData();
  static void deleteData();

  static map<string, std::unique_ptr<TrActorEntityType>> m_actorTypes;
  static map<string, std::unique_ptr<TrBuildingEntityType>> m_buildingTypes;
  static map<string, std::unique_ptr<TrPlantEntityType>> m_plantTypes;

  static map<string, std::unique_ptr<TrFormula>> m_formulas;
  static map<string, std::unique_ptr<TrItem>> m_items;
};

#endif
