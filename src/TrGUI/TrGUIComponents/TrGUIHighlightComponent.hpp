#pragma once
/**
 * TrGUIHighlightComponent.hpp
 *
 * Victor Jiao
 *
 * Enables components to have highlights
 */

#include "TrGUIComponent.hpp"
#include "TrGUIMouseComponent.hpp"
#include <memory>

class TrGUIHighlightComponent : public TrGUIComponent {
 public:
  SDL_Color m_highlightColor;
  SDL_Color m_highlightDarkColor;
  SDL_Color m_highlightLightColor;

  TrGUIHighlightComponent() {
    m_highlightColor.r = 0xFF;
    m_highlightColor.g = 0xE9;
    m_highlightColor.b = 0xBC;
    m_highlightColor.a = 0xFF;

    m_highlightDarkColor = convertColorType(0x88AA8844);
    m_highlightLightColor = convertColorType(0x88FFFFDC);
  };

  virtual void update(TrGame *game);
};

