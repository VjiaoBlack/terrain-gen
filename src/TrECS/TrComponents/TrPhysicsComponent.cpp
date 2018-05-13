/**
 * TrPHysicsComponent.cpp
 */

#include "TrPhysicsComponent.hpp"

void TrPhysicsComponent::update(TrGame *game, TrEntity *entity) {
  m_vx += m_ax;
  m_vy += m_ay;

  m_x += m_vx;
  m_y += m_vy;
}
