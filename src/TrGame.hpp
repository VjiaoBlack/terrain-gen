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
#include <iterator>
#include <list>
#include <random>
#include <vector>

#include "TrMap/TrMap.hpp"

using namespace std;

class TrGameLoop;
class TrMainMenuLoop;
class TrRenderLoop;

class TrGame {
 public:
  SDL_Window* m_SDLWindow;
  SDL_Renderer* m_SDLRenderer;

  TrMap* m_map;
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
  SDL_Texture* m_mapTexture;

  TTF_Font* m_font;
  TTF_Font* m_menuFont;

  int m_xOff;
  int m_yOff;
  int m_speed;

  TrRenderLoop* m_gameStateTransition;
  list<TrRenderLoop*> m_gameStateStack;

  // random number generator for rain
  random_device m_yrandDevice;
  default_random_engine m_yrandEngine;
  uniform_int_distribution<int> m_yrandDist;

  random_device m_xrandDevice;
  default_random_engine m_xrandEngine;
  uniform_int_distribution<int> m_xrandDist;

  // framerate counter
  clock_t m_deltaTime;
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
