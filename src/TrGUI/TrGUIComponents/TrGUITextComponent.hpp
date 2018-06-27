#pragma once
/**
 * TrGUITextComponent.hpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include "TrGUIComponent.hpp"
#include "TrGUIGraphicsComponent.hpp"

class TrGUITextComponent : public TrGUIComponent {
 public:
  std::string m_text;
  sdl_texture_pt m_textTexture;
  SDL_Rect m_destRect;
  SDL_Color m_textColor;

  TrGUITextComponent(TrGame* game, std::string &&label, SDL_Rect r);

  virtual void update(TrGame* game);
};

