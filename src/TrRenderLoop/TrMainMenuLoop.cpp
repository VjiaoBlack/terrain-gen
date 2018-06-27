/**
 * TrMainMenuLoop.cpp
 */

#include "TrMainMenuLoop.hpp"
#include "../TrECS/TrSystems/TrEntitySystem.hpp"
#include "TrGameLoop.hpp"
#include "TrTransitionLoop.hpp"
#include <TrGUI/TrGUIEntity.hpp>

#include "TrECS/TrEntities.hpp"

#include "TrGUI/TrGUIComponents/TrGUIGraphicsComponent.hpp"

TrMainMenuLoop::TrMainMenuLoop(TrGame *game) {
  sdl_surface_pt textSurface0(TTF_RenderText_Solid(
      game->m_font.get(), m_titleText.c_str(), textColor0));
  sdl_surface_pt textSurface1(TTF_RenderText_Solid(
      game->m_font.get(), m_titleText.c_str(), textColor1));
  sdl_surface_pt textSurface2(TTF_RenderText_Solid(
      game->m_font.get(), m_titleText.c_str(), textColor2));

  text0.reset(
      SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface0.get()));
  text1.reset(
      SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface1.get()));
  text2.reset(
      SDL_CreateTextureFromSurface(game->m_SDLRenderer, textSurface2.get()));
  text_width = textSurface1->w;
  text_height = textSurface1->h;

  m_GUISystem = std::make_unique<TrGUISystem>();

  SDL_Rect r =
      (SDL_Rect){sz(K_DISPLAY_SIZE_X / 2 / K_DISPLAY_SCALE - 120 / 2),
                 sz(K_DISPLAY_SIZE_Y / 2 / K_DISPLAY_SCALE - text_height / 2),
                 sz(120), sz(K_DISPLAY_SIZE_Y / 2 / K_DISPLAY_SCALE)};

  vector<string> mylabels = {"New Game", "Load Game", "Quit"};
  m_GUIMenu = TrGUIEntity::addVerticalMenu(game, m_GUISystem.get(), r,
                                           std::move(mylabels));

  vector<string> labels = {"New Game", "Load Game", "Quit"};
}

TrMainMenuLoop::~TrMainMenuLoop() = default;

TrRenderLoop *TrMainMenuLoop::update(TrGame *game) {
  game->m_map->update(game->m_keysDown);

  if (m_GUIMenu->get<TrGUIContainerComponent>()
          ->m_buttons[0]
          ->get<TrGUIClickableComponent>()
          ->m_activated) {
    auto gameLoop = std::make_shared<TrGameLoop>(game);
    game->m_gameStateTransition = TrTransitionLoop::makePushLoop(
        game, game->m_gameStateStack.back(), gameLoop);

    game->m_gameStateStack.push_back(gameLoop);
  }

  if (m_GUIMenu->get<TrGUIContainerComponent>()
          ->m_buttons[2]
          ->get<TrGUIClickableComponent>()
          ->m_activated) {
    game->m_quit = true;
  }

  return nullptr;
}

void TrMainMenuLoop::render(TrGame *game) {
  renderTextureWithOffset(game->m_SDLRenderer, game->m_mapTexture.get(),
                          game->m_xOff, game->m_yOff, K_DISPLAY_SCALE);

  SDL_Rect fillRect = {0, 0, sz(K_DISPLAY_SIZE_X), sz(K_DISPLAY_SIZE_Y)};

  SDL_SetRenderDrawColor(game->m_SDLRenderer, 0x00, 0x00, 0x00, 0x40);

  SDL_RenderFillRect(game->m_SDLRenderer, &fillRect);

  // text
  SDL_Rect renderQuad = {
      sz(-4 + K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
      sz(-4 + K_DISPLAY_SIZE_Y / 4 - text_height * K_DISPLAY_SCALE / 2),
      text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
      text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text0.get(), nullptr, &renderQuad);

  renderQuad = {
      sz(4 + K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
      sz(4 + K_DISPLAY_SIZE_Y / 4 - text_height * K_DISPLAY_SCALE / 2),
      text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
      text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text2.get(), nullptr, &renderQuad);

  renderQuad = {sz(K_DISPLAY_SIZE_X / 2 - text_width * K_DISPLAY_SCALE / 2),
                sz(K_DISPLAY_SIZE_Y / 4 - text_height * K_DISPLAY_SCALE / 2),
                text_width * sz(K_DISPLAY_SIZE_X / K_MAP_SIZE_X),
                text_height * sz(K_DISPLAY_SIZE_Y / K_MAP_SIZE_Y)};

  SDL_RenderCopy(game->m_SDLRenderer, text1.get(), nullptr, &renderQuad);

  m_GUISystem->update(game);
}
