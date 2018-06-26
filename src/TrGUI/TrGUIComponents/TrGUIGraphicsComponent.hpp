#pragma once
/**
 * TrGUIGraphicsComponent.hpp
 *
 * Victor Jiao
 *
 * Enables components to be drawn (at all)
 */

#include <string>
#include "../../TrGraphics.hpp"
#include "TrGUIComponent.hpp"

class TrGame;

class TrGUIGraphicsComponent : public TrGUIComponent {
 public:
  SDL_Rect m_srcRect{};
  SDL_Rect m_destRect{};
  SDL_Rect m_rect{};

  SDL_Color m_buttonColor;
  SDL_Color m_drawColor;

  virtual void update(TrGame *game);
};

