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

  for (int i = 0; i < numButtons; i++) {
    // TODO: make this spacing metric even over all of m_rect
    m_menu->m_buttons[i] = new TrGUIButton(
        game, {sz(8) + m_menu->m_rect.x +
                   (i * (m_menu->m_rect.w + m_menu->m_spacing - sz(16)) /
                    (numButtons)),
               sz(8) + m_menu->m_rect.y, sz(8), sz(8)},
        labels[i]);
  }

  SDL_Surface* image;
  image = IMG_Load("res/icons.png");
  printf("ASDF\n");
  if (!image) {
    printf("IMG_Load: %s\n", IMG_GetError());
  }

  SDL_Texture* map = SDL_CreateTextureFromSurface(game->m_SDLRenderer, image);

  m_menu->m_labelTexture = map;
  m_menu->m_buttons[0]->m_labelTexture = map;
  m_menu->m_buttons[1]->m_labelTexture = map;
  m_menu->m_buttons[2]->m_labelTexture = map;
  m_menu->m_buttons[3]->m_labelTexture = map;
  m_menu->m_buttons[4]->m_labelTexture = map;
  m_menu->m_buttons[5]->m_labelTexture = map;

  m_menu->m_buttons[0]->m_srcRect = {0, 0, 8, 8};
  m_menu->m_buttons[1]->m_srcRect = {0, 8, 8, 8};
  m_menu->m_buttons[2]->m_srcRect = {0, 16, 8, 8};
  m_menu->m_buttons[3]->m_srcRect = {0, 24, 8, 8};
  m_menu->m_buttons[4]->m_srcRect = {8, 24, 8, 8};
  m_menu->m_buttons[5]->m_srcRect = {24, 24, 8, 8};
  m_menu->m_srcRect = {8, 0, 24, 24};

  m_menu->m_buttons[0]->m_destRect = m_menu->m_buttons[0]->m_rect;
  m_menu->m_buttons[1]->m_destRect = m_menu->m_buttons[1]->m_rect;
  m_menu->m_buttons[2]->m_destRect = m_menu->m_buttons[2]->m_rect;
  m_menu->m_buttons[3]->m_destRect = m_menu->m_buttons[3]->m_rect;
  m_menu->m_buttons[4]->m_destRect = m_menu->m_buttons[4]->m_rect;
  m_menu->m_buttons[5]->m_destRect = m_menu->m_buttons[5]->m_rect;
  m_menu->m_destRect = m_menu->m_rect;
}

TrRenderLoop* TrGameLoop::update(TrGame* game) {
  m_menu->update();

  game->m_map->update(game->m_keysDown);

  for (auto key : game->m_keysDown) {
    switch (key) {
      case SDLK_v:
        TrTransitionLoop* transition = new TrTransitionLoop(game);
        transition->setTarget(new TrMainMenuLoop(game));
        transition->setSource(this);
        return transition;
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
