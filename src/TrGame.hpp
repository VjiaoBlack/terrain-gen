#pragma once

#include <set>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <iostream>
#include <random>
#include <vector>

#include "TrMap/TrMap.hpp"
#include "TrRenderLoop/TrGameLoop.hpp"
#include "TrRenderLoop/TrMainMenuLoop.hpp"

class TrGame {
 public:
  SDL_Window* m_SDLWindow;
  SDL_Renderer* m_SDLRenderer;

  TrMap* m_map;
  // setup for rendering loop
  SDL_Event m_SDLEvent;
  bool m_quit = false;

  set<int> m_keysDown;
  set<char> m_buttonsDown;

  // create texture for map
  SDL_Texture* m_mapTexture;

  TTF_Font* m_font;

  // TrMap* m_terrain;

  int m_xOff = 0;
  int m_yOff = 0;

  // TODO: help
  const int c_pixelSize = sz(K_DISPLAY_SIZE_X) / K_MAP_SIZE_X;

  int m_speed = 1;

  TrRenderLoop* m_gameState;

  // initialize random number generator for rain
  std::random_device m_yrandDevice;
  std::default_random_engine m_yrandEngine;
  std::uniform_int_distribution<int> m_yrandDist;

  std::random_device m_xrandDevice;
  std::default_random_engine m_xrandEngine;
  std::uniform_int_distribution<int> m_xrandDist;

  // initialize framerate counter
  clock_t m_deltaTime = 0;
  unsigned int m_frames = 0;
  double m_frameRate = 30;

  TrGame();
  ~TrGame();

  void handleInput();
  void handleKey(int SDLKey);
  void run();

  void mainMenu();
};
