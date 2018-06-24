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
  std::string m_label;
  sdl_texture_pt m_labelTexture;

  SDL_Rect m_srcRect{};
  SDL_Rect m_destRect{};
  SDL_Rect m_rect{};

  SDL_Color m_buttonColor;
  SDL_Color m_drawColor;

  virtual void update(TrGUIEntity *ent, TrGame *game) {
    float sx, sy;
    SDL_Rect m_shadowRect = makeInsetRect(m_rect, 1, 1, 0, 0);
    SDL_Rect m_lightRect = makeInsetRect(m_rect, 0, 0, 1, 1);
    SDL_Rect m_innerBevelRect = makeInsetRect(m_rect, 1, 1, 1, 1);

    SDL_RenderGetScale(game->m_SDLRenderer, &sx, &sy);
    SDL_RenderSetScale(game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                       sz(K_DISPLAY_SCALE));

//    setMainButtonDrawColor();
//    if (m_mouseInside) {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xFF, 0xE9, 0xBC, 0xFF);
//    } else {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0xFF);
//    }
    SDL_SetRenderDrawColor(game->m_SDLRenderer, m_drawColor.r, m_drawColor.g, m_drawColor.b,
                           m_drawColor.a);

    SDL_RenderFillRect(game->m_SDLRenderer, &m_rect);

//    if (m_mouseInside) {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0x88);
//    } else {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0x58, 0x3E, 0x09, 0x88);
//    }
    SDL_RenderFillRect(game->m_SDLRenderer, &m_shadowRect);

//    if (m_mouseInside) {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xFF, 0xFF, 0xDC, 0x88);
//    } else {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xFF, 0xE9, 0xBC, 0x88);
//    }
    SDL_RenderFillRect(game->m_SDLRenderer, &m_lightRect);

//    setMainButtonDrawColor();
    SDL_SetRenderDrawColor(game->m_SDLRenderer, m_drawColor.r, m_drawColor.g, m_drawColor.b,
                           m_drawColor.a);

    SDL_RenderFillRect(game->m_SDLRenderer, &m_innerBevelRect);

    if (m_labelTexture) {
      SDL_RenderCopy(game->m_SDLRenderer, m_labelTexture.get(), &m_srcRect, &m_destRect);
    }

    SDL_RenderSetScale(game->m_SDLRenderer, sx, sy);
  }
};

