#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrComponents.hpp"

using namespace std;

/**
 * @brief interface for all physics components
 */
class TrPhysicsComponent : public TrComponent {
 public:

  TrPhysicsComponent(float x, float y) {}
  TrPhysicsComponent() : TrPhysicsComponent(0, 0) {}

  /**
   * @brief calculates the physics stuffs
   */
  template<class C>
  void update(TrGame *game, C *entity);
};

template<class C>
void TrPhysicsComponent::update(TrGame *game, C *entity) {
//  entity->m_vx += m_ax;
//  entity->m_vy += m_ay;

  entity->m_x += entity->m_vx;
  entity->m_y += entity->m_vy;

  entity->m_rect.x = round(entity->m_x);
  entity->m_rect.y = round(entity->m_y);
}
