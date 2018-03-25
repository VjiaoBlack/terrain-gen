/**
 * TrGUIMenu.cpp
 */

#include "TrGUIMenu.hpp"
#include "../TrGame.hpp"

TrGUIMenu::TrGUIMenu(TrGame *game, SDL_Rect rect, vector<string> labels) {
  m_rect = {rect.x / K_DISPLAY_SCALE, rect.y / K_DISPLAY_SCALE,
            rect.w / K_DISPLAY_SCALE, rect.h / K_DISPLAY_SCALE};
  this->m_game = game;
  int numButtons = labels.size();

  m_buttons.resize(numButtons);

  for (int i = 0; i < numButtons; i++) {
    // TODO: make this spacing metric even over all of m_rect
    m_buttons[i] = new TrGUIButton(
        game, {m_rect.x, m_rect.y + i * (m_rect.h + m_spacing) / (numButtons),
               m_rect.w, (m_rect.h + m_spacing) / (numButtons)-m_spacing},
        labels[i]);
  }
}

TrGUIMenu::~TrGUIMenu() {
  for (int i = 0; i < m_buttons.size(); i++) {
    delete m_buttons[i];
  }
}

void TrGUIMenu::draw() {
  float sx, sy;
  SDL_RenderGetScale(m_game->m_SDLRenderer, &sx, &sy);

  SDL_RenderSetScale(m_game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  if (m_texture) {
    SDL_RenderCopy(m_game->m_SDLRenderer, m_texture, &m_srcRect, &m_destRect);

  } else {
    SDL_SetRenderDrawColor(m_game->m_SDLRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawRect(m_game->m_SDLRenderer, &m_rect);
  }
  for (auto button : m_buttons) {
    button->draw();
  }

  SDL_RenderSetScale(m_game->m_SDLRenderer, sx, sy);
}

void TrGUIMenu::update() {
  for (auto button : m_buttons) {
    button->update();
  }
}
