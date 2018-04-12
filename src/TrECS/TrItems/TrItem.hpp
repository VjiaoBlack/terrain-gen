#pragma once

#include <string>

/**
 * TrItem.hpp
 */

using namespace std;

class TrItem {
 public:
  string m_name;

  TrItem(string name) : m_name(name) {}

  virtual ~TrItem() {}
};