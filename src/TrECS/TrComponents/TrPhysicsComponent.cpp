/**
 * TrPHysicsComponent.cpp
 */

#include "TrPhysicsComponent.hpp"

template<class C>
void TrPhysicsComponent::update(TrGame *game, C *entity) {
  m_vx += m_ax;
  m_vy += m_ay;

  m_x += m_vx;
  m_y += m_vy;
}
