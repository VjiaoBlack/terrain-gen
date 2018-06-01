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
class TrPhysicsComponent : public MyComponent {
 public:
  float m_x, m_y;
  float m_vx, m_vy;
  float m_ax, m_ay;

  TrPhysicsComponent(float x, float y)
      : m_x(x), m_y(y), m_vx(0), m_vy(0), m_ax(0), m_ay(0) {}
  TrPhysicsComponent() : TrPhysicsComponent(0, 0) {}

  /**
   * @brief calculates the physics stuffs
   */
  template<class C>
  void update(TrGame *game, C *entity);
};

template<class C>
void TrPhysicsComponent::update(TrGame *game, C *entity) {
  m_vx += m_ax;
  m_vy += m_ay;

  m_x += m_vx;
  m_y += m_vy;
}
