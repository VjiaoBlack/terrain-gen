
#include "TrGame.hpp"
#include "../include/fft/fft.h"
#include "TrData/TrData.hpp"
#include "TrRenderLoop/TrGameLoop.hpp"
#include "TrRenderLoop/TrMainMenuLoop.hpp"
#include "TrRenderLoop/TrRenderLoop.hpp"

TrGame::TrGame() {
  TrData data = TrData::getInstance();
  m_gameStateTransition = nullptr;

  // Initialize
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize - SDL Error: %s\n", SDL_GetError());
    exit(1);
  }

  // Create window
  m_SDLWindow = SDL_CreateWindow("test_driving", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, sz(K_DISPLAY_SIZE_X),
                                 sz(K_DISPLAY_SIZE_Y), SDL_WINDOW_SHOWN);
  if (m_SDLWindow == nullptr) {
    printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
    exit(1);
  }

  // Create renderer for window
  m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
  if (m_SDLRenderer == nullptr) {
    printf("Renderer could not be created - SDL Error: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_SetRenderDrawBlendMode(m_SDLRenderer, SDL_BLENDMODE_BLEND);

  if (TTF_Init() == -1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
  }

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    printf("SDL_image could not initialize! SDL_image Error: %s\n",
           IMG_GetError());
    exit(2);
  }

  // setup for rendering loop
  m_quit = false;

  // create texture for map
  m_mapTexture =
      SDL_CreateTexture(m_SDLRenderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STATIC, K_MAP_SIZE_X, K_MAP_SIZE_Y);

  m_map = new TrMap(K_MAP_SIZE_Y, K_MAP_SIZE_X);

  m_xOff = 0;
  m_yOff = 0;

  m_speed = 1;

  // initialize random number generator for rain
  m_xrandEngine = std::default_random_engine(m_xrandDevice());
  m_yrandEngine = std::default_random_engine(m_yrandDevice());
  // TODO: _X vs _Y??
  m_xrandDist = std::uniform_int_distribution<int>(0, K_MAP_SIZE_X);
  m_yrandDist = std::uniform_int_distribution<int>(0, K_MAP_SIZE_Y);

  // initialize framerate counter
  m_deltaTime = 0;
  m_frames = 0;
  m_frameRate = 30;
  // double  averageFrameTimeMilliseconds = 33.333;

  m_font = TTF_OpenFont("anirb.ttf", 26);
  if (!m_font) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    // handle error
    exit(2);
  }

  m_menuFont = TTF_OpenFont("anirb.ttf", 14);
  if (!m_menuFont) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    // handle error
    exit(2);
  }

  // setup game loop
  m_gameStateStack.push_back(new TrMainMenuLoop(this));
}

TrGame::~TrGame() {
  delete m_map;

  SDL_DestroyTexture(m_mapTexture);
  SDL_DestroyRenderer(m_SDLRenderer);
  SDL_DestroyWindow(m_SDLWindow);

  if (m_font) {
    TTF_CloseFont(m_font);
  }
  if (m_menuFont) {
    TTF_CloseFont(m_menuFont);
  }
  while (m_gameStateStack.size()) {
    delete m_gameStateStack.back();
    m_gameStateStack.pop_back();
  }

  if (m_gameStateTransition) {
    delete m_gameStateTransition;
  }

  TTF_Quit();
  SDL_Quit();
}

void TrGame::handleKey(int SDLKey) {
  switch (SDLKey) {
    case SDLK_q:
      m_quit = true;
      break;
    case SDLK_LSHIFT:
      m_speed = 10;
      break;
    case SDLK_RSHIFT:
      m_speed = 1;
      break;
    case SDLK_UP:
      m_yOff -= K_DISPLAY_SCALE * m_speed;
      break;
    case SDLK_DOWN:
      m_yOff += K_DISPLAY_SCALE * m_speed;
      break;
    case SDLK_LEFT:
      m_xOff -= K_DISPLAY_SCALE * m_speed;
      break;
    case SDLK_RIGHT:
      m_xOff += K_DISPLAY_SCALE * m_speed;
      break;
  }
}

void TrGame::run() {
  while (!m_quit) {
    clock_t beginFrame = clock();

    // Update keysDown and buttonsDown
    handleInput();

    // Assuming that order of processing doesn't matter.
    for (auto it = m_keysDown.begin(); it != m_keysDown.end(); it++) {
      handleKey(*it);
    }

    SDL_UpdateTexture(m_mapTexture, nullptr, m_map->m_color->m_data,
                      K_MAP_SIZE_X * sizeof(uint32_t));

    // clear screen
    SDL_SetRenderDrawColor(m_SDLRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(m_SDLRenderer);

    if (m_gameStateTransition) {
      m_gameStateTransition->render(this);
      TrRenderLoop* loop = m_gameStateTransition->update(this);
      if (!loop) {
        m_gameStateTransition = nullptr;
      }
    } else {
      m_gameStateStack.back()->render(this);
      TrRenderLoop* loop = m_gameStateStack.back()->update(this);
    }

    // update screen
    SDL_RenderPresent(m_SDLRenderer);

    // do render stuff
    clock_t endFrame = clock();

    m_deltaTime += endFrame - beginFrame;
    m_frames++;

    // display FPS
    // TODO: fix (make this update faster)
    if (clockToMilliseconds(m_deltaTime) > 100.0) {  // every second
      m_frameRate = (double)m_frames;                // more stable
      m_frames = 0;
      m_deltaTime -= CLOCKS_PER_SEC;

      std::cout << "fps: " << m_frameRate << "        \n";
      std::flush(std::cout);
    }

    float calcMs = clockToMilliseconds(endFrame - beginFrame);
    if (calcMs < 16.6) {
      usleep(1000 * ((int)(16.6 - calcMs)));
    }
  }
}

void TrGame::handleInput() {
  m_keysDownPrev.empty();
  m_buttonsDownPrev.empty();
  m_keysDownPrev = m_keysDown;
  m_buttonsDownPrev = m_buttonsDown;

  while (SDL_PollEvent(&m_SDLEvent) != 0) {
    if (m_SDLEvent.type == SDL_QUIT) {
      m_quit = true;
    } else if (m_SDLEvent.type == SDL_KEYDOWN) {
      m_keysDown.insert(m_SDLEvent.key.keysym.sym);
    } else if (m_SDLEvent.type == SDL_KEYUP) {
      m_keysDown.erase(m_SDLEvent.key.keysym.sym);
    } else if (m_SDLEvent.type == SDL_MOUSEBUTTONDOWN) {
      m_buttonsDown.insert(m_SDLEvent.button.button);
    } else if (m_SDLEvent.type == SDL_MOUSEBUTTONUP) {
      m_buttonsDown.erase(m_SDLEvent.button.button);
    } else if (m_SDLEvent.type == SDL_MOUSEMOTION) {
      m_mouseX = m_SDLEvent.motion.x / K_DISPLAY_SCALE;
      m_mouseY = m_SDLEvent.motion.y / K_DISPLAY_SCALE;
    }
  }
}
