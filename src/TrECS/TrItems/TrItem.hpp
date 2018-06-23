#pragma once

#include <string>
#include <utility>
/**
 * TrItem.hpp
 */

using namespace std;

class TrItem {
 public:
  string m_name;

  explicit TrItem(string name) : m_name(std::move(std::move(name))) {}

  virtual ~TrItem() = default;
};
