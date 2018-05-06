#pragma once

/**
 * TrEntityType.hpp
 */
class TrGraphicsComponent;
class TrFootprintComponent;

/**
 * @brief Anything that is a building
 */
class TrEntityType {
 public:
  TrGraphicsComponent* m_graphics;

  explicit TrEntityType(TrGraphicsComponent* graphics) : m_graphics(graphics){};

  virtual ~TrEntityType() { delete m_graphics; }
};
