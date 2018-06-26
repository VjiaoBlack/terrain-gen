#pragma once
/**
 * TrGUIGraphicsComponent.hpp
 *
 * Victor Jiao
 *
 * Enables components to be drawn (at all)
 */

#include <TrGUI/TrGUIComponents/TrGUIGraphicsComponent.hpp>
#include <TrGame.hpp>

void TrGUIGraphicsComponent::update(TrGame *game) {

//    printf("wat\n");
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
    SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0xFF);
//    }
//    SDL_SetRenderDrawColor(game->m_SDLRenderer, m_drawColor.r, m_drawColor.g, m_drawColor.b,
//                           m_drawColor.a);

  SDL_RenderFillRect(game->m_SDLRenderer, &m_rect);

//    if (m_mouseInside) {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xAA, 0x88, 0x44, 0x88);
//    } else {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0x58, 0x3E, 0x09, 0x88);
//    }
//    SDL_RenderFillRect(game->m_SDLRenderer, &m_shadowRect);

//    if (m_mouseInside) {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xFF, 0xFF, 0xDC, 0x88);
//    } else {
//      SDL_SetRenderDrawColor(game->m_SDLRenderer, 0xFF, 0xE9, 0xBC, 0x88);
//    }
//    SDL_RenderFillRect(game->m_SDLRenderer, &m_lightRect);

//    setMainButtonDrawColor();
//    SDL_SetRenderDrawColor(game->m_SDLRenderer, m_drawColor.r, m_drawColor.g, m_drawColor.b,
//                           m_drawColor.a);

//    SDL_RenderFillRect(game->m_SDLRenderer, &m_innerBevelRect);

  SDL_RenderSetScale(game->m_SDLRenderer, sx, sy);
}

