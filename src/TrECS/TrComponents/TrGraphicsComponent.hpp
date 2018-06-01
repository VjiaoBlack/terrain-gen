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
class TrGraphicsComponent : public MyComponent {
 public:
  SDL_Texture *m_texture;
  SDL_Color m_color;

  explicit TrGraphicsComponent(SDL_Texture *texture)
      : m_texture(texture), m_color{0, 0, 0, 0} {};
  explicit TrGraphicsComponent(SDL_Color color) : m_texture(nullptr), m_color(color) {};
  TrGraphicsComponent() : TrGraphicsComponent(nullptr) {}

  ~TrGraphicsComponent() override = default;

  /**
   * @brief draws the given entity
   */
  template<class C>
  void update(TrGame *game, C *entity);
};


template<class C>
void TrGraphicsComponent::update(TrGame *game, C *entity) {
  float sx, sy;

//  cout << "DRAW " << (int)m_color.r << " " << (int)m_color.g << " " << entity->m_rect.x << " "<<
//                                entity->m_rect
//      .y << endl;

  SDL_RenderGetScale(game->m_SDLRenderer, &sx, &sy);
  SDL_RenderSetScale(game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  if (m_texture) {
    SDL_RenderCopy(game->m_SDLRenderer, m_texture, nullptr, &(entity->m_rect));
  } else {
    SDL_SetRenderDrawColor(game->m_SDLRenderer, m_color.r, m_color.g, m_color.b,
                           m_color.a);
    SDL_RenderFillRect(game->m_SDLRenderer, &(entity->m_rect));
  }
  SDL_RenderSetScale(game->m_SDLRenderer, sx, sy);
}

