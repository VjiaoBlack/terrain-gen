/**
 * TrGameLoop.cpp
 */

#include "TrGameLoop.hpp"
#include "TrMainMenuLoop.hpp"
#include "TrTransitionLoop.hpp"
#include "../TrECS/TrSystems/TrEntitySystem.hpp"

#include "../TrECS/MyEntities.hpp"

class TrMainMenuLoop;

TrGameLoop::TrGameLoop(TrGame *game) {
  vector<string> labels = {" ", " ", " ", " ", " ", " "};
  vector<string> sublabels = {"A", "B", "C", "D", "E", "F"};
  int numButtons = labels.size();

  // lots of this is hardcoded
  // TODO: somehow fix the hardcoding
  m_menu = TrGUIMenu::MakeHorizontalMenu(
      game,
      (SDL_Rect) {sz(K_DISPLAY_SIZE_X / 2 / K_DISPLAY_SCALE - 104 / 2),
                  sz(K_DISPLAY_SIZE_Y / K_DISPLAY_SCALE - 32), sz(104), sz(24)},
      labels);

  m_menu->m_spacing = 8;

  vector<unique_ptr<TrGUIButton>> tempButtons(numButtons);

  for (int i = 0; i < numButtons; i++) {
    delete m_menu->m_buttons[i];
    // TODO: make this spacing metric even over all of m_rect
    tempButtons[i] = make_unique<TrGUIButton>(TrGUIButton(
        game, {sz(8) + m_menu->m_rect.x +
            (i * (m_menu->m_rect.w + m_menu->m_spacing - sz(16)) /
                (numButtons)),
               sz(8) + m_menu->m_rect.y, sz(8), sz(8)},
        labels[i]));
  }

  SDL_Surface *image;
  image = IMG_Load("res/icons.png");
  if (!image) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }

  m_map.reset(SDL_CreateTextureFromSurface(game->m_SDLRenderer, image));
  SDL_FreeSurface(image);

  image = IMG_Load("res/game_toolbar.png");
  if (!image) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }

  m_menu->m_texture.reset(SDL_CreateTextureFromSurface(game->m_SDLRenderer, image));
  m_menu->m_srcRect = {0, 0, 104, 24};
  m_menu->m_destRect = m_menu->m_rect;

  SDL_FreeSurface(image);

  tempButtons[0]->m_srcRect = {0, 0, 8, 8};
  tempButtons[1]->m_srcRect = {0, 8, 8, 8};
  tempButtons[2]->m_srcRect = {0, 16, 8, 8};
  tempButtons[3]->m_srcRect = {0, 24, 8, 8};
  tempButtons[4]->m_srcRect = {8, 24, 8, 8};
  tempButtons[5]->m_srcRect = {24, 24, 8, 8};

  SDL_Rect rect = m_menu->m_rect;
  rect.y -= 32;

  m_menu->m_destRect = m_menu->m_rect;

  for (int i = 0; i < 6; i++) {
    tempButtons[i]->m_texture = m_map;
    tempButtons[i]->m_destRect = tempButtons[i]->m_rect;

    TrGUIDropdownMenu *tempMenu = new TrGUIDropdownMenu(
        game, move(tempButtons[i]),
        TrGUIMenu::MakeHorizontalMenu(game, rect, sublabels));

    tempMenu->m_menu->m_texture = m_menu->m_texture;
    tempMenu->m_menu->m_destRect = rect;
    tempMenu->m_menu->m_srcRect = {0, 0, 104, 24};

    for (int j = 0; j < 6; j++) {
      tempMenu->m_menu->m_buttons[j]->m_rect = {
          sz(8) + tempMenu->m_menu->m_rect.x +
              (j * (rect.w + tempMenu->m_menu->m_spacing - sz(16)) /
                  (numButtons)),
          sz(8) + rect.y, sz(8), sz(8)};
      tempMenu->m_menu->m_buttons[j]->m_destRect =
          tempMenu->m_menu->m_buttons[j]->m_rect;
      m_menu->m_buttons[i] = tempMenu;
    }
  }
}

TrGameLoop::~TrGameLoop() = default;

TrRenderLoop *TrGameLoop::update(TrGame *game) {
  m_menu->update();

  game->m_map->update(game->m_keysDown);

  for (auto key : game->m_keysDown) {
    switch (key) {
      case SDLK_v:auto penu = game->m_gameStateStack.end();
        penu--;
        penu--;

        game->m_gameStateTransition = TrTransitionLoop::makePopLoop(
            game, game->m_gameStateStack.back(), *penu);
        game->m_gameStateStack.pop_back();

        break;
    }
  }

  return this;
}

void TrGameLoop::render(TrGame *game) {
  renderTextureWithOffset(game->m_SDLRenderer, game->m_mapTexture.get(), game->m_xOff,
                          game->m_yOff, K_DISPLAY_SCALE);

  m_menu->draw();
}
