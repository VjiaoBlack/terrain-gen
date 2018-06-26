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

  virtual void update(TrGame *game);
};

