/**
 * TrGUITextComponent.cpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include <TrGUI/TrGUIComponents/TrGUITextComponent.hpp>
#include <TrGame.hpp>

TrGUITextComponent::TrGUITextComponent(TrGame *game, std::string label,
                                       SDL_Rect r)
    : m_text(label), m_destRect(r) {
  m_textColor.a = 0xFF;
  m_textColor.r = 0x3C;
  m_textColor.g = 0x37;
  m_textColor.b = 0x14;

  sdl_surface_pt textSurface(
      TTF_RenderText_Solid(game->m_menuFont.get(), label.c_str(), m_textColor));

  m_destRect.w = textSurface->w;
  m_destRect.h = textSurface->h;
  m_destRect.x = r.x + (r.w - m_destRect.w) / 2;
  m_destRect.y = r.y + (r.h - m_destRect.h) / 2;

  m_textTexture.reset(
      SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface.get()));
}

void TrGUITextComponent::update(TrGame *game) {
  float sx, sy;
  SDL_RenderGetScale(game->m_SDLRenderer, &sx, &sy);
  SDL_RenderSetScale(game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));
  if (m_textTexture) {
    SDL_RenderCopy(game->m_SDLRenderer, m_textTexture.get(), NULL, &m_destRect);
  }

  SDL_RenderSetScale(game->m_SDLRenderer, sx, sy);
}
