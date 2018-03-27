#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../Game.hpp"
#include "TrComponents.hpp"

using namespace std;

/**
 * @brief interface for all components
 * @details using the Entity Component System design pattern
 */
class TrComponent {
 public:
  virtual ~TrComponent(){};
  virtual void update(TrEntity* entity) = 0;
};

/**
 * @brief interface for all graphics components
 */
class TrGraphicsComponent : TrComponent {
 public:
  SDL_Texture* m_texture;

  TrGraphicsComponent(SDL_Texture* texture) : m_texture(texture){};
  TrGraphicsComponent() : TrGraphicsComponent(nullptr) {}

  virtual ~TrGraphicsComponent(){};

  /**
   * @brief draws the given entity
   */
  virtual void update(TrEntity* entity) = 0;
};

/**
 * @brief interface for all physics components
 */
class TrPhysicsComponent : TrComponent {
 public:
  float m_x, m_y;
  float m_vx, m_vy;
  float m_ax, m_ay;

  TrPhysicsComponent(float x, float y)
      : m_x(x), m_y(y), m_vx(0), m_vy(0), m_ax(0), m_ay(0) {}

  /**
   * @brief calculates the physics stuffs
   */
  virtual void update(TrEntity* entity) = 0;
};

/**
 * @brief Buildings have variable footprints, this keeps track of them
 */
class TrBuildingFootprintComponent : TrComponent {
 public:
  int m_w;
  int m_h;
  bool* m_footprint;

  TrBuildingFootprintComponent(int w, int h, bool* footprint)
      : m_w(w), m_h(h), m_footprint(footprint){};
};

/**
 * @brief holds information about a intended destination to go to
 * @details urgency is on the scale of 0 to 7
 */
struct tr_target_t {
  SDL_Point m_loc;
  uint8_t urgency;
};

/**
 * @brief interface for all planning components
 */
class TrPlanningComponent : TrComponent {
 public:
  set<tr_target_t> m_targets;

  TrPlanningComponent(){};
  virtual ~TrPlanningComponent(){};

  /**
   * @brief based on the game and entity state, figure out something to do
   */
  virtual void update(TrEntity* entity) = 0;
};
