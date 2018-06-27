#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrComponents.hpp"

/**
 * @brief interface for all physics components
 */
class TrPhysicsComponent : public TrComponent {
 public:
  double m_x;
  double m_y;
  double m_vx;
  double m_vy;

  TrPhysicsComponent(double x, double y) : m_x(x), m_y(y),
                                           m_vx(0), m_vy(0) {}
  TrPhysicsComponent() : TrPhysicsComponent(0, 0) {}

  template<class C>
  void update(TrGame *game, C *entity);
};

template<class C>
void TrPhysicsComponent::update(TrGame *game, C *entity) {
  double dx = game->m_mouseX - m_x;
  double dy = game->m_mouseY - m_y;
  double dd = sqrt(dx * dx + dy * dy);

  m_vx = 0.05 * dx / dd;
  m_vy = 0.05 * dy / dd;

  m_x += m_vx;
  m_y += m_vy;

  entity->m_rect.x = round(m_x);
  entity->m_rect.y = round(m_y);
}
