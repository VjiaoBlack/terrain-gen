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
  TrFootprintComponent* m_footprint;

  TrEntityType(TrGraphicsComponent* graphics,
               TrFootprintComponent* footprint)
      : m_graphics(graphics), m_footprint(footprint){};

  ~TrEntityType() { 
  	delete m_footprint;
  	delete m_graphics;
  }
};
