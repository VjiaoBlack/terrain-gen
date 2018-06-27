#pragma once

#include <string>
#include <utility>
/**
 * TrItem.hpp
 */

class TrItem {
 public:
  string m_name;

  explicit TrItem(string&& name) : m_name(std::move(name)) {}

  virtual ~TrItem() = default;
};
