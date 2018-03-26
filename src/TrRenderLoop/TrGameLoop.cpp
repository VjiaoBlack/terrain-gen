/**
 * TrGameLoop.cpp
 */

#include "TrGameLoop.hpp"
#include "../TrGame.hpp"
#include "TrMainMenuLoop.hpp"
#include "TrTransitionLoop.hpp"

class TrMainMenuLoop;

TrGameLoop::TrGameLoop(TrGame* game) {
  vector<string> labels = {" ", " ", " ", " ", " ", " "};
  int numButtons = labels.size();

  // lots of this is hardcoded
  // TODO: somehow fix the hardcoding
  m_menu = new TrGUIMenu(
      game, (SDL_Rect){sz(K_DISPLAY_SIZE_X / 2 - 104 * K_DISPLAY_SCALE / 2),
                       sz(K_DISPLAY_SIZE_Y - 32 * K_DISPLAY_SCALE),
                       sz(104 * K_DISPLAY_SCALE), sz(24 * K_DISPLAY_SCALE)},
      labels);

  m_menu->m_spacing = 8;

  vector<TrGUIButton*> tempButtons(numButtons);

  for (int i = 0; i < numButtons; i++) {
    delete m_menu->m_buttons[i];
    // TODO: make this spacing metric even over all of m_rect
    tempButtons[i] = new TrGUIButton(
        game, {sz(8) + m_menu->m_rect.x +
                   (i * (m_menu->m_rect.w + m_menu->m_spacing - sz(16)) /
                    (numButtons)),
               sz(8) + m_menu->m_rect.y, sz(8), sz(8)},
        labels[i]);
  }

  SDL_Surface* image;
  image = IMG_Load("res/icons.png");
  if (!image) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }

  m_map = SDL_CreateTextureFromSurface(game->m_SDLRenderer, image);

  m_menu->m_texture = m_map;
  tempButtons[0]->m_texture = m_map;
  tempButtons[1]->m_texture = m_map;
  tempButtons[2]->m_texture = m_map;
  tempButtons[3]->m_texture = m_map;
  tempButtons[4]->m_texture = m_map;
  tempButtons[5]->m_texture = m_map;

  tempButtons[0]->m_srcRect = {0, 0, 8, 8};
  tempButtons[1]->m_srcRect = {0, 8, 8, 8};
  tempButtons[2]->m_srcRect = {0, 16, 8, 8};
  tempButtons[3]->m_srcRect = {0, 24, 8, 8};
  tempButtons[4]->m_srcRect = {8, 24, 8, 8};
  tempButtons[5]->m_srcRect = {24, 24, 8, 8};
  m_menu->m_srcRect = {8, 0, 24, 24};

  tempButtons[0]->m_destRect = tempButtons[0]->m_rect;
  tempButtons[1]->m_destRect = tempButtons[1]->m_rect;
  tempButtons[2]->m_destRect = tempButtons[2]->m_rect;
  tempButtons[3]->m_destRect = tempButtons[3]->m_rect;
  tempButtons[4]->m_destRect = tempButtons[4]->m_rect;
  tempButtons[5]->m_destRect = tempButtons[5]->m_rect;
  m_menu->m_destRect = m_menu->m_rect;

  m_menu->m_buttons[0] = new TrGUIDropdownMenu(
      game, tempButtons[0],
      new TrGUIMenu(game, tempButtons[0]->m_rect, vector<string>({"a", "b"})));
  m_menu->m_buttons[1] = new TrGUIDropdownMenu(
      game, tempButtons[1],
      new TrGUIMenu(game, tempButtons[1]->m_rect, vector<string>({"a", "b"})));
  m_menu->m_buttons[2] = new TrGUIDropdownMenu(
      game, tempButtons[2],
      new TrGUIMenu(game, tempButtons[2]->m_rect, vector<string>({"a", "b"})));
  m_menu->m_buttons[3] = new TrGUIDropdownMenu(
      game, tempButtons[3],
      new TrGUIMenu(game, tempButtons[3]->m_rect, vector<string>({"a", "b"})));
  m_menu->m_buttons[4] = new TrGUIDropdownMenu(
      game, tempButtons[4],
      new TrGUIMenu(game, tempButtons[4]->m_rect, vector<string>({"a", "b"})));
  m_menu->m_buttons[5] = new TrGUIDropdownMenu(
      game, tempButtons[5],
      new TrGUIMenu(game, tempButtons[5]->m_rect, vector<string>({"a", "b"})));
  m_menu->m_destRect = m_menu->m_rect;
}

TrGameLoop::~TrGameLoop() {
  // SDL_DestroyTexture(m_map);
  delete m_menu;
}

TrRenderLoop* TrGameLoop::update(TrGame* game) {
  m_menu->update();

  game->m_map->update(game->m_keysDown);

  for (auto key : game->m_keysDown) {
    switch (key) {
      case SDLK_v:
        auto loopIt = *(std::next(game->m_gameStateStack.begin(),
                                  game->m_gameStateStack.size() - 2));
        // delete game->m_gameStateStack.back();
        game->m_gameStateStack.pop_back();
        game->m_gameStateStack.pop_back();
        TrTransitionLoop* transition = new TrTransitionLoop(game, this, loopIt);
        // game->m_gameStateStack.push_back(transition);
        break;
    }
  }

  return nullptr;
}

void TrGameLoop::render(TrGame* game) {
  renderTextureWithOffset(game->m_SDLRenderer, game->m_mapTexture, game->m_xOff,
                          game->m_yOff, game->c_pixelSize);
  m_menu->draw();
}
