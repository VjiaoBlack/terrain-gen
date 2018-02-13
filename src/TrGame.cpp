
#include "TrGame.hpp"

TrGame::TrGame() {
    // Initialize
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create window 
    m_SDLWindow = SDL_CreateWindow("test_driving",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              sz(K_DISPLAY_SIZE), sz(K_DISPLAY_SIZE), SDL_WINDOW_SHOWN);
    if (m_SDLWindow == NULL) {
        printf("Window could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Create renderer for window 
    m_SDLRenderer = SDL_CreateRenderer(m_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (m_SDLRenderer == NULL) {
        printf("Renderer could not be created - SDL Error: %s\n", SDL_GetError());
        exit(1);
    }
    


    
    if (TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }

    // setup for rendering loop
    m_quit = false;

    // create texture for map
    m_mapTexture = SDL_CreateTexture(m_SDLRenderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, K_MAP_SIZE, K_MAP_SIZE);

    m_map = new TrMap(K_MAP_SIZE, K_MAP_SIZE);


    m_xOff = 0;
    m_yOff = 0;

    m_speed = 1;

    // initialize random number generator for rain
    m_randEngine = std::default_random_engine(m_randDevice());
    m_randDist = std::uniform_int_distribution<int>(0, K_MAP_SIZE);

    // initialize framerate counter
    m_deltaTime = 0;
    m_frames = 0;
    m_frameRate = 30;
    // double  averageFrameTimeMilliseconds = 33.333;


    m_font = TTF_OpenFont("neris.thin.ttf", 16);
    if(!m_font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
}

TrGame::~TrGame() {
    // delete m_terrain;

    TTF_Quit();
    SDL_DestroyTexture(m_mapTexture);
    SDL_DestroyRenderer(m_SDLRenderer);
    SDL_DestroyWindow(m_SDLWindow);
    SDL_Quit();
}

void TrGame::handleKey(int SDLKey) {
    switch(SDLKey) {
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
            m_yOff -= c_pixelSize * m_speed;
            break;
        case SDLK_DOWN:
            m_yOff += c_pixelSize * m_speed; 
            break;
        case SDLK_LEFT:
            m_xOff -= c_pixelSize * m_speed;
            break;
        case SDLK_RIGHT:
            m_xOff += c_pixelSize * m_speed;
            break;
    }
}

void TrGame::run() {

    // create random map
    

    // render menu loop
    

    // render game loop
    

    int score = 10;

    std::string score_text = "athena ATHENA: 1234567890";       
    SDL_Color textColor = { 0, 0, 0, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(m_font, score_text.c_str(), textColor);
    SDL_Texture* text = SDL_CreateTextureFromSurface(m_SDLRenderer, textSurface);
    int text_width = textSurface->w;
    int text_height = textSurface->h;
    SDL_FreeSurface(textSurface);
    SDL_Rect renderQuad = { 20, 20, text_width, text_height };
    SDL_RenderCopy(m_SDLRenderer, text, NULL, &renderQuad);
    // SDL_DestroyTexture(text);


    while (!m_quit) {
        clock_t beginFrame = clock();

        SDL_UpdateTexture(m_mapTexture, NULL, m_map->m_color->m_data, K_MAP_SIZE * sizeof(uint32_t));

        // Update keysDown and buttonsDown
        handleInput();

        // Assuming that order of processing doesn't matter.
        for (auto it = m_keysDown.begin(); it != m_keysDown.end(); it++) {
            handleKey(*it);
        }


        m_map->update(m_keysDown);


        if (m_xOff < 0)
            m_xOff += sz(K_DISPLAY_SIZE);
        if (m_xOff > sz(K_DISPLAY_SIZE))
            m_xOff -= sz(K_DISPLAY_SIZE);
        if (m_yOff < 0)
            m_yOff += sz(K_DISPLAY_SIZE);
        if (m_yOff > sz(K_DISPLAY_SIZE))
            m_yOff -= sz(K_DISPLAY_SIZE);

        // clear screen
        SDL_SetRenderDrawColor(m_SDLRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_SDLRenderer);
            
        renderTextureWithOffset(m_SDLRenderer, m_mapTexture, m_xOff, m_yOff, c_pixelSize);





        SDL_Rect renderQuad = { 0, 0, text_width * sz(K_DISPLAY_SIZE / K_MAP_SIZE), text_height * sz(K_DISPLAY_SIZE / K_MAP_SIZE) };
        SDL_RenderCopy(m_SDLRenderer, text, NULL, &renderQuad);





        // update screen
        SDL_RenderPresent(m_SDLRenderer);

        clock_t endFrame = clock();

        m_deltaTime += endFrame - beginFrame;
        m_frames++;

        // display FPS
        if (clockToMilliseconds(m_deltaTime)>1000.0){ //every second
            m_frameRate = (double)m_frames; //more stable
            m_frames = 0;
            m_deltaTime -= CLOCKS_PER_SEC;
            // averageFrameTimeMilliseconds  = 1000.0/(frameRate==0?0.001:frameRate);

            std::cout <<"fps: " << m_frameRate << "        \r" ;
            std::flush(std::cout);
        } 

        float calcMs = clockToMilliseconds(endFrame - beginFrame);
        if (calcMs < 16.6) {
            // limit to 60 FPS
            usleep(1000 * ((int) (16.6 - calcMs)));
        }
    }
}

void TrGame::handleInput() {
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
            if (m_buttonsDown.count(SDL_BUTTON_LEFT) &&
                m_SDLEvent.motion.x < sz(K_DISPLAY_SIZE)) {
                int mouseX = m_SDLEvent.motion.x / c_pixelSize;
                int mouseY = m_SDLEvent.motion.y / c_pixelSize;
                // m_terrain->m_height->m_data[mouseY * K_MAP_SIZE + mouseX] = 0;
            }

            if (m_buttonsDown.count(SDL_BUTTON_RIGHT) &&
                m_SDLEvent.motion.x < sz(K_DISPLAY_SIZE)) {
                int mouseX = m_SDLEvent.motion.x / c_pixelSize;
                int mouseY = m_SDLEvent.motion.y / c_pixelSize;
                // m_terrain->m_height->m_data[mouseY * K_MAP_SIZE + mouseX] = 255;
            }
        }
    }
}

