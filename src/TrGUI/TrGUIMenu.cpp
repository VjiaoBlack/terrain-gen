/**
 * TrGUIMenu.cpp
 */

#include "TrGUIMenu.hpp"

TrGUIMenu::TrGUIMenu(TrGame *game, SDL_Rect rect, vector<string> labels) {
  m_rect = rect;

  this->m_game = game;
  int numButtons = labels.size();

  m_buttons.resize(numButtons);

}

unique_ptr<TrGUIMenu> TrGUIMenu::MakeHorizontalMenu(TrGame *game, SDL_Rect rect,
                                                    vector<string> labels) {
  unique_ptr<TrGUIMenu> menu(new TrGUIMenu(game, rect, labels));
  int numButtons = labels.size();

  for (int i = 0; i < labels.size(); i++) {
    menu->m_buttons[i] = new TrGUIButton(
        game,
        {rect.x + i * (rect.w + menu->m_spacing) / (numButtons), rect.y,
         (rect.w + menu->m_spacing) / (numButtons) - menu->m_spacing, rect.h},
        labels[i]);
  }
  return menu;
}

unique_ptr<TrGUIMenu> TrGUIMenu::MakeVerticalMenu(TrGame *game, SDL_Rect rect,
                                                  vector<string> labels) {
  unique_ptr<TrGUIMenu> menu(new TrGUIMenu(game, rect, labels));
  int numButtons = labels.size();

  for (int i = 0; i < labels.size(); i++) {
    menu->m_buttons[i] = new TrGUIButton(
        game,
        {rect.x, rect.y + i * (rect.h + menu->m_spacing) / (numButtons), rect.w,
         (rect.h + menu->m_spacing) / (numButtons) - menu->m_spacing},
        labels[i]);
  }
  return menu;
}

TrGUIMenu::~TrGUIMenu() {
  for (auto &m_button : m_buttons) {
    delete m_button;
  }

}

void TrGUIMenu::draw() {
  float sx, sy;
  SDL_RenderGetScale(m_game->m_SDLRenderer, &sx, &sy);

  SDL_RenderSetScale(m_game->m_SDLRenderer, sz(K_DISPLAY_SCALE),
                     sz(K_DISPLAY_SCALE));

  if (m_texture) {
    SDL_RenderCopy(m_game->m_SDLRenderer, m_texture.get(), &m_srcRect, &m_destRect);

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
