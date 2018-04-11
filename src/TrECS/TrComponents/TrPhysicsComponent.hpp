
#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrComponents.hpp"

class TrEntity;

using namespace std;

/**
 * @brief interface for all physics components
 */
class TrPhysicsComponent : public TrComponent {
 public:
  float m_x, m_y;
  float m_vx, m_vy;
  float m_ax, m_ay;

  TrPhysicsComponent(float x, float y)
      : m_x(x), m_y(y), m_vx(0), m_vy(0), m_ax(0), m_ay(0) {}

  /**
   * @brief calculates the physics stuffs
   */
  virtual void update(TrGame* game, TrEntity* entity) = 0;
};
