#pragma once

#include <set>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include <iostream>
#include <iterator>
#include <list>
#include <random>
#include <vector>
#include <memory>

#include "TrMap/TrMap.hpp"
#include "TrGraphics.hpp"

using namespace std;

class TrGameLoop;
class TrMainMenuLoop;
class TrRenderLoop;
class TrEntitySystem;

class TrGame {
 public:
  TrWindow m_SDLWindow;

  TrRenderer m_SDLRenderer;

  unique_ptr<TrMap> m_map;

  unique_ptr<TrEntitySystem> m_entSystem;

  // setup for rendering loop
  SDL_Event m_SDLEvent;
  bool m_quit;

  set<int> m_keysDown;
  set<int> m_keysDownPrev;
  set<char> m_buttonsDown;
  set<char> m_buttonsDownPrev;
  int m_mouseX;
  int m_mouseY;

  // create texture for map
  sdl_texture_pt m_mapTexture = TrSDL::null_texture();

  ttf_font_pt m_font = TrSDL::null_font();
  ttf_font_pt m_menuFont = TrSDL::null_font();

  int m_xOff;
  int m_yOff;
  int m_speed;

  unique_ptr<TrRenderLoop> m_gameStateTransition;
  list<shared_ptr<TrRenderLoop>> m_gameStateStack;

  // random number generator for rain
  random_device m_yrandDevice;
  default_random_engine m_yrandEngine;
  uniform_int_distribution<int> m_yrandDist;

  random_device m_xrandDevice;
  default_random_engine m_xrandEngine;
  uniform_int_distribution<int> m_xrandDist;

  // framerate counter
  double m_deltaTime;
  unsigned int m_frames;
  double m_frameRate;

  TrGame();
  ~TrGame();

  void setupSDL();

  void handleInput();
  void handleKey(int SDLKey);
  void run();

  void mainMenu();
};
