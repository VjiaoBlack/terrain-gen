#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrComponents.hpp"
class TrEntity;

using namespace std;

/**
 * @brief interface for all graphics components
 */
class TrGraphicsComponent : public TrComponent {
 public:
  SDL_Texture* m_texture;
  SDL_Color m_color;

  TrGraphicsComponent(SDL_Texture* texture)
      : m_texture(texture), m_color{0, 0, 0, 0} {};
  TrGraphicsComponent(SDL_Color color) : m_texture(nullptr), m_color(color){};
  TrGraphicsComponent() : TrGraphicsComponent(nullptr) {}

  virtual ~TrGraphicsComponent(){};

  /**
   * @brief draws the given entity
   */
  virtual void update(TrGame* game, TrEntity* entity) = 0;
};
