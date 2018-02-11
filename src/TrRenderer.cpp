
#include "TrRenderer.hpp"

TrRenderer::TrRenderer() {
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
    
    // setup for rendering loop
    m_quit = false;

    // create texture for map
    m_mapTexture = SDL_CreateTexture(m_SDLRenderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, K_MAP_SIZE, K_MAP_SIZE);

    m_terrain = new TrMap(K_MAP_SIZE, K_MAP_SIZE);

    // m_terrain->m_height->at(0,0) = 0.5;
    // m_terrain->m_height->diamondSquare(K_MAP_SIZE, 0.8);
    // terrain is doubley stretched
    m_terrain->m_height->perlinNoise(K_MAP_SIZE, 8, 2.0, 1.0);

    for (int i = 0; i < m_terrain->m_rows; i++) {
        for (int j = 0; j < m_terrain->m_cols; j++) {
            m_terrain->m_height->at(i,j) -= 0.5;
            m_terrain->m_height->at(i,j) -= 0.5;
            m_terrain->m_height->at(i,j) *= 1.5;


            m_terrain->m_height->at(i,j) *= fabs(m_terrain->m_height->at(i,j));

            m_terrain->m_height->at(i,j) += 0.5;

            if (m_terrain->m_height->at(i,j) > 1.0) {
                m_terrain->m_height->at(i,j) = 1.0;
            } else if (m_terrain->m_height->at(i,j) < 0.0) {
                m_terrain->m_height->at(i,j) = 0.0;
            }



        }
    }

    pair<double, double> minMax = m_terrain->m_height->getMinMax();
    cout << "min: " << minMax.first << endl;
    cout << "max: " << minMax.second << endl;

    m_terrain->updateColors();

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

}

TrRenderer::~TrRenderer() {
    delete m_terrain;

    SDL_DestroyTexture(m_mapTexture);
    SDL_DestroyRenderer(m_SDLRenderer);
    SDL_DestroyWindow(m_SDLWindow);
    SDL_Quit();
}

void TrRenderer::handleKey(int SDLKey) {
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
        case SDLK_u:
             for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    m_terrain->m_height->m_pixels[i * K_MAP_SIZE + j] -= (double)m_speed / 255.0;
                }
            }
            m_terrain->updateColors();
            break;
        case SDLK_i:
             for (int i = 0; i < K_MAP_SIZE; i++) {
                for (int j = 0; j < K_MAP_SIZE; j++) {
                    m_terrain->m_height->m_pixels[i * K_MAP_SIZE + j] += (double)m_speed / 255.0;
                }
            }
            m_terrain->updateColors();
            break;
        case SDLK_b:
            for (int i = 0; i < m_speed; i++) {
                m_terrain->m_height->boxBlur();
            }
            m_terrain->updateColors();
            break;
        case SDLK_r:
            // create rain  
            for (int i = 0; i < m_speed * K_MAP_SIZE * K_MAP_SIZE / 1000; i++) {
                m_terrain->m_water->at(m_randDist(m_randEngine),m_randDist(m_randEngine)) += 0.01;
            }
            m_terrain->updateColors();
            break;
        case SDLK_d:
            m_terrain->m_water->set(0.0);
            m_terrain->updateColors();
            break;
        case SDLK_w:
            // animate water engine
            for (int i = 0; i < m_speed; i++) {
                m_terrain->updateWater(m_keysDown.count(SDLK_e));
            }
            m_terrain->updateColors();
            break;
        case SDLK_n:
            // reclaculate normals
            m_terrain->updateNormals();
            m_terrain->updateColors();
            break;
        case SDLK_m:
            // update water!! omg 
            for (int i = 0; i < m_speed; i++) {
                m_terrain->updateMoisture();
                m_terrain->updateColors();
            }
            break;
        case SDLK_k:
            // toggle moisture / not
            // or else it would blink tooo fastttt
            usleep(100000);
            m_terrain->m_useMoisture = !m_terrain->m_useMoisture;
            m_terrain->updateColors();
            break;
        case SDLK_j:
            m_terrain->updateWind();
            break;
        case SDLK_s:
            // Save the map: color and heightmap
            saveMap();
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

void TrRenderer::run() {
    while (!m_quit) {
        clock_t beginFrame = clock();

        SDL_UpdateTexture(m_mapTexture, NULL, m_terrain->m_diffuse->m_pixels, K_MAP_SIZE * sizeof(uint32_t));

        // Update keysDown and buttonsDown
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
                    m_terrain->m_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 0;
                }

                if (m_buttonsDown.count(SDL_BUTTON_RIGHT) &&
                    m_SDLEvent.motion.x < sz(K_DISPLAY_SIZE)) {
                    int mouseX = m_SDLEvent.motion.x / c_pixelSize;
                    int mouseY = m_SDLEvent.motion.y / c_pixelSize;
                    m_terrain->m_height->m_pixels[mouseY * K_MAP_SIZE + mouseX] = 255;
                }
            }
        }

        // Assuming that order of processing doesn't matter.
        for (auto it = m_keysDown.begin(); it != m_keysDown.end(); it++) {
            handleKey(*it);
        }

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

void TrRenderer::saveMap() {
    SDL_Surface *surface, *surface2;

    surface = SDL_CreateRGBSurface(0, K_MAP_SIZE, K_MAP_SIZE, K_RGBA_BYTES,
                                   K_R_MASK, K_G_MASK, K_B_MASK, K_A_MASK);

    SDL_LockSurface(surface);

    memcpy(surface->pixels, m_terrain->m_diffuse->m_pixels, K_MAP_SIZE * K_MAP_SIZE * 4);


    SDL_UnlockSurface(surface);

    IMG_SavePNG(surface, "color.png");
    SDL_FreeSurface(surface);

    TrPixels<uint32_t> map_height(K_MAP_SIZE, K_MAP_SIZE);


    for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            map_height.at(i,j) = floor(m_terrain->m_height->at(i,j) * 255);
        }
    }


    for (int i = 0; i < K_MAP_SIZE; i++) {
        for (int j = 0; j < K_MAP_SIZE; j++) {
            map_height.m_pixels[i * K_MAP_SIZE + j] |= 
                    map_height.m_pixels[i * K_MAP_SIZE + j] << 8;
            map_height.m_pixels[i * K_MAP_SIZE + j] |= 
                    map_height.m_pixels[i * K_MAP_SIZE + j] << 16;
            map_height.m_pixels[i * K_MAP_SIZE + j] |= 
                    map_height.m_pixels[i * K_MAP_SIZE + j] << 24;
            map_height.m_pixels[i * K_MAP_SIZE + j] = 
                    map_height.m_pixels[i * K_MAP_SIZE + j] | 0xFF000000;
        }
    }


    surface2 = SDL_CreateRGBSurface(0, K_MAP_SIZE, K_MAP_SIZE, K_RGBA_BYTES,
                                    K_R_MASK, K_G_MASK, K_B_MASK, K_A_MASK);

    SDL_LockSurface(surface2);
    memcpy(surface2->pixels, map_height.m_pixels, K_MAP_SIZE * K_MAP_SIZE * sizeof(uint32_t));
    SDL_UnlockSurface(surface2);

    IMG_SavePNG(surface2, "height.png");

    SDL_FreeSurface(surface2);
}