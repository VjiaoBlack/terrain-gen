/**
 * TrGUITextComponent.cpp
 *
 * Victor Jiao
 *
 * Makes entities clickable
 */

#include <TrGUI/TrGUIComponents/TrGUITextComponent.hpp>
#include <TrGame.hpp>

TrGUITextComponent::TrGUITextComponent(TrGame *game, std::string &&label, SDL_Rect r) : m_text
                                                                                          (label),
                                                                                      m_destRect(r) {
  m_textColor.a = 0xFF;
  m_textColor.r = 0x00;
  m_textColor.g = 0x00;
  m_textColor.b = 0x00;

  sdl_surface_pt textSurface0(TTF_RenderText_Solid(game->m_font.get(), label.c_str(),
                                                   m_textColor));

  m_textTexture.reset(SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface0.get()));
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

