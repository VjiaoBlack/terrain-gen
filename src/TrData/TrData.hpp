#pragma once

/**
 * TrData.hpp
 */
#include <iostream>
#include <map>
#include <string>

#include "../../include/fast-cpp-csv-parser/csv.h"

#include "../TrECS/TrEntityTypes/TrEntityType.hpp"
#include "../TrECS/TrFormulas/TrFormula.hpp"
#include "../TrECS/TrItems/TrItem.hpp"

using namespace std;

static class TrData {
 public:
  static void loadData();
  static void deleteData();

  static map<string, TrEntityType *> m_entityTypes;
  static map<string, TrFormula *> m_formulas;
  static map<string, TrItem *> m_items;
};
