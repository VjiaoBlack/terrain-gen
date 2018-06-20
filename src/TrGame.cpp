
#include "TrGame.hpp"
#include "TrData/TrData.hpp"
#include "TrRenderLoop/TrGameLoop.hpp"
#include "TrRenderLoop/TrMainMenuLoop.hpp"
#include "TrECS/TrSystems/TrEntitySystem.hpp"

#include <deque>
//#include <TrECS/TrEntityTypes.hpp>
#include <TrECS/TrEntities.hpp>
#include "TrECS/MyEntities.hpp"

void TrGame::setupSDL() {
  // Initialize
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize - SDL Error: %s\n", SDL_GetError());
    exit(1);
  }

  // Create window
  m_SDLWindow = TrWindow(SDL_CreateWindow("athena", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, sz(K_DISPLAY_SIZE_X),
                                          sz(K_DISPLAY_SIZE_Y), SDL_WINDOW_SHOWN));
  if (m_SDLWindow == nullptr) {
    printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
    exit(1);
  }

  // Create renderer for window
  m_SDLRenderer = TrRenderer(SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED));
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
}

TrGame::TrGame()
    : m_quit(false),
      m_gameStateTransition(nullptr),
      m_deltaTime(0),
      m_frames(0),
      m_frameRate(30),
      m_xOff(0),
      m_yOff(0),
      m_speed(1) {

  TrData::loadData();


  /**
   * Test area: test game functions
   */

  this->setupSDL();

  // create texture for map
  m_mapTexture.reset(SDL_CreateTexture(m_SDLRenderer, SDL_PIXELFORMAT_ARGB8888,
                                       SDL_TEXTUREACCESS_STATIC, K_MAP_SIZE_X, K_MAP_SIZE_Y));

  m_map = std::make_unique<TrMap>(K_MAP_SIZE_Y, K_MAP_SIZE_X, this);

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

  m_font.reset(TTF_OpenFont("anirb.ttf", 26));
  if (!m_font) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    // handle error
    exit(2);
  }

  m_menuFont.reset(TTF_OpenFont("anirb.ttf", 14));
  if (!m_menuFont) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    // handle error
    exit(2);
  }

  int cell_rad = 7;

  // okay, are we going to set up some plants?
  m_entSystem = std::move(make_unique<TrEntitySystem>());
  int* indices = new int[(K_MAP_SIZE_X / cell_rad) * (K_MAP_SIZE_Y / cell_rad)];

  memset(indices, -1, sizeof(int) * (K_MAP_SIZE_X / cell_rad) * (K_MAP_SIZE_Y / cell_rad));



  int rows = K_MAP_SIZE_Y / cell_rad;
  int cols = K_MAP_SIZE_X / cell_rad;

//  for (int r = 0; r < rows; r++) {
//    for (int c = 0; c < cols; c++) {
////      cout << indices[r * cols + c] << endl;
//    }
//  }

  // create initial plant
  SDL_Rect rect = {rand() % (K_MAP_SIZE_X - 2), rand() % (K_MAP_SIZE_Y - 2), 3, 3};
  rect.y += 100;

  auto plant = TrData::m_plantTypes["tree"]->make();
  plant->m_rect = rect;
  m_entSystem->m_plants.push_back(move(plant));

  vector<int> active_list;

  active_list.push_back(0);

  int active_list_start = 0;


  while (active_list_start < active_list.size()) {

    int index = active_list[active_list_start++];

    auto parent_rec = m_entSystem->m_plants[index]->m_rect;

    for (int i = 0; i < 5; i++) {
      rect = m_entSystem->m_plants[index]->m_rect;

      int rad = rand() % cell_rad + cell_rad;
      double theta = rand() % 360;
      rect.x += round(rad * sin((theta / 180.0) * M_PI));
      rect.y += round(rad * cos((theta / 180.0) * M_PI));

      if (rect.x < 0 || rect.x >= K_MAP_SIZE_X ||
          rect.y < 0 || rect.y >= K_MAP_SIZE_Y) {
//        cout << "cont1" << endl;

        continue;
      }

      int t_r = rect.y / cell_rad;
      int t_c = rect.x / cell_rad;

      if (indices[t_r * cols + t_c] > -1) {
//        cout << "cont2 " << m_entSystem->m_plants[index]->m_rect.y / cell_rad << " " <<
//             m_entSystem->m_plants[index]->m_rect.x / cell_rad << " " <<
//             t_r << " " << t_c << endl;
        continue;
      }

      // check collisions
      bool valid = true;
      for (int d_r = glm::max(t_r - 1, 0); d_r <= glm::min(t_r + 1, rows - 1); d_r++) {
        if (!valid) {
          break;
        }
        for (int d_c = glm::max(t_c - 1, 0); d_c <= glm::min(t_c + 1, cols - 1); d_c++) {
          if (indices[d_r * cols + d_c] > -1) {
            // make sure its far away enough
            auto currec = m_entSystem->m_plants[indices[d_r * cols + d_c]]->m_rect;
            if (glm::distance(vec2(rect.x, rect.y), vec2(currec.x, currec.y)) <= cell_rad) {
              valid = false;
              break;
            }
          }
        }
      }


      if (valid) {
        active_list.push_back(m_entSystem->m_plants.size());

        auto plant = TrData::m_plantTypes["tree"]->make();
        plant->m_rect = rect;
        m_entSystem->m_plants.push_back(std::move(plant));

        indices[t_r * cols + t_c] = (int) m_entSystem->m_plants.size() - 1;
      }
    }
  }

  for (int i = m_entSystem->m_plants.size() - 1; i >= 0; i--) {
    auto rect = m_entSystem->m_plants[i]->m_rect;


    if (m_map->m_height->at(rect.y, rect.x) < 0.5 ||
        m_map->m_height->at(rect.y, rect.x) > 0.65) {
      m_entSystem->m_plants.erase(m_entSystem->m_plants.begin() + i);
    }
  }

  // setup game loop
  auto tempmenu = make_shared<TrMainMenuLoop>(this);
  m_gameStateStack.push_back(move(tempmenu));
}


TrGame::~TrGame() {
  while (!m_gameStateStack.empty()) {
    m_gameStateStack.pop_back();
  }
}

void TrGame::handleKey(int SDLKey) {
  switch (SDLKey) {
    case SDLK_q:m_quit = true;
      break;
    case SDLK_LSHIFT:m_speed = 10;
      break;
    case SDLK_RSHIFT:m_speed = 1;
      break;
    case SDLK_UP:m_yOff -= K_DISPLAY_SCALE * m_speed;
      break;
    case SDLK_DOWN:m_yOff += K_DISPLAY_SCALE * m_speed;
      break;
    case SDLK_LEFT:m_xOff -= K_DISPLAY_SCALE * m_speed;
      break;
    case SDLK_RIGHT:m_xOff += K_DISPLAY_SCALE * m_speed;
      break;
    default:break;
  }
}

void TrGame::run() {
  while (!m_quit) {
    clock_t beginFrame = clock();

    // Update keysDown and buttonsDown
    handleInput();

    // Assuming that order of processing doesn't matter.
    for (int it : m_keysDown) {
      handleKey(it);
    }

    SDL_UpdateTexture(m_mapTexture.get(), nullptr, m_map->m_color->m_data,
                      K_MAP_SIZE_X * sizeof(uint32_t));

    // clear screen
    SDL_SetRenderDrawColor(m_SDLRenderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(m_SDLRenderer);

    if (m_gameStateTransition) {
      m_gameStateTransition->render(this);
      TrRenderLoop *loop = m_gameStateTransition->update(this);
      if (!loop) {
        m_gameStateTransition.reset(nullptr);
      }
    } else {
      m_gameStateStack.back()->render(this);
      TrRenderLoop *loop = m_gameStateStack.back()->update(this);
    }

    // update screen
    SDL_RenderPresent(m_SDLRenderer);

    // do render stuff
    clock_t endFrame = clock();

    m_deltaTime = clockToMilliseconds(endFrame - beginFrame);
    double rfps = (1000.0 / m_deltaTime);
    // display rFPS (rendering FPS)
    std::cout << "rFPS: " << rfps << "       \r";
    fflush(stdout);
    if (m_deltaTime < (1000.0 / 30.0)) {
      usleep(1000 * ((1000.0 / 30.0) - m_deltaTime));
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
