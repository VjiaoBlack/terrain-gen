#pragma once

/**
 * TrFormula.hpp
 */

#include <map>
#include <string>

using namespace std;

class TrItem;

class TrFormula {
 public:
  TrFormula() = default;

  map<TrItem *, int> m_inputs;
  map<TrItem *, int> m_outputs;
  map<TrItem *, int> m_byproducts;

  virtual ~TrFormula() = default;
};