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
  SDL_Rect m_rect;

  SDL_Color m_buttonColor;
  SDL_Color m_buttonDarkColor;
  SDL_Color m_buttonLightColor;

  SDL_Color m_drawColor;
  SDL_Color m_drawDarkColor;
  SDL_Color m_drawLightColor;

  TrGUIGraphicsComponent() = delete;

  TrGUIGraphicsComponent(SDL_Rect rect) : m_rect(rect) {
    m_buttonColor = convertColorType(0xFFAA8844);
    m_buttonLightColor = convertColorType(0x88FFE9BC);
    m_buttonDarkColor = convertColorType(0x88583E09);

    m_drawColor = m_buttonColor;
    m_drawDarkColor = m_buttonDarkColor;
    m_drawLightColor = m_buttonLightColor;
  };

  virtual void update(TrGame *game);
};

