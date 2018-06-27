#pragma once

#include <string>
#include <utility>
/**
 * TrItem.hpp
 */

class TrItem {
 public:
  std::string m_name;

  explicit TrItem(std::string&& name) : m_name(std::move(name)) {}

  virtual ~TrItem() = default;
};
