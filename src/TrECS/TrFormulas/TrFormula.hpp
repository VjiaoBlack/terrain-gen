#pragma once

/**
 * TrFormula.hpp
 */

#include <map>
#include <string>

class TrItem;

class TrFormula {
 public:
  TrFormula() = default;

  // TODO: be careful, can't use regular map with unordered objects
//  map<TrItem, int> m_inputs;
//  map<TrItem, int> m_outputs;
//  map<TrItem, int> m_byproducts;

  virtual ~TrFormula() = default;
};