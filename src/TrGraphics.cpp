/**
 * TrGraphics
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include "TrGraphics.hpp"


void TrMap::updateColors() {
    int threshold[9] = {70, 100, 117, 132, 148, 165, 180, 198, 218};

    uint32_t colors[9] = {0xFF1A2B56,0xFF253C78,0xFF3A5BAA,
                          0xFFEEDDBB,0xFF77BC49,0xFF58A327,
                          0xFF28771F,0xFF210E04,0xFF5B3F31};

    // uint32_t colors[9] = {0xFF664433,0xFFBB8866,0xFFEEDDBB,
    //                       0xFFEEDDBB,0xFF77BC49,0xFF58A327,
    //                       0xFF28771F,0xFF210E04,0xFF5B3F31};

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            // m_diffuse->at(i,j) = floor(m_height->at(i,j) * 255.0);
            for (int c = 0; c < 9; c++) {
                if (m_height->get(i,j) * 255 < threshold[c]) {
                    m_diffuse->set(i,j,colors[c]);
                    break;
                }

                if (m_height->get(i,j) * 255 >= threshold[8]) {
                    m_diffuse->set(i,j,0xFFFFFFFF);
                    break;
                }

                
            }

            // if (m_water->at(i,j) > 0.001) {
            //     m_diffuse->at(i,j) = 0xFF3A5BAA;
            // }

            // if (m_water->at(i,j) > 0.05) {
            //     m_diffuse->at(i,j) = 0xFF253C78;
            // }

            // if (m_water->at(i,j) > 0.2) {
            //     m_diffuse->at(i,j) = 0xFF1A2B56;
            // }
            



            if (m_water->at(i,j) > 0.001) {
                float height = m_water->at(i,j) + m_height->at(i,j);
                m_diffuse->at(i,j) = 0xFF000000 + floor(height * 255.0);
            }



            

            

            // uint32_t max = 255 - (m_diffuse->get(i,j) & 0xFF);
            // uint32_t water = ceil(m_water->at(i,j) * 255.0);
            // uint32_t diff = water > max ? max : water;
            // m_diffuse->at(i,j) = 0xFFFFFFFF & m_diffuse->at(i,j) + water;
            // m_diffuse->at(i,j) = 0xFF000000 + water;
        }
    }
}

void renderTextureWithOffset(SDL_Renderer* renderer, SDL_Texture* texture, int xOff, int yOff, int pixelSize) {
	SDL_Rect SrcR;
    SDL_Rect DestR;

    SrcR.x = xOff / pixelSize;
    SrcR.y = yOff / pixelSize;
    SrcR.w = K_MAP_SIZE - xOff / pixelSize;
    SrcR.h = K_MAP_SIZE - yOff / pixelSize;

    DestR.x = 0;
    DestR.y = 0;
    DestR.w = sz(K_DISPLAY_SIZE) - xOff;
    DestR.h = sz(K_DISPLAY_SIZE) - yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);


    SrcR.x = 0;
    SrcR.y = 0;
    SrcR.w = xOff / pixelSize;
    SrcR.h = yOff / pixelSize;

    DestR.x = sz(K_DISPLAY_SIZE) - xOff;
    DestR.y = sz(K_DISPLAY_SIZE) - yOff;
    DestR.w = xOff;
    DestR.h = yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);


    SrcR.x = 0;
    SrcR.y = yOff / pixelSize;
    SrcR.w = xOff / pixelSize;
    SrcR.h = K_MAP_SIZE;

    DestR.x = sz(K_DISPLAY_SIZE) - xOff;
    DestR.y = 0;
    DestR.w = xOff;
    DestR.h = sz(K_DISPLAY_SIZE) - yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);


    SrcR.x = xOff / pixelSize;
    SrcR.y = 0;
    SrcR.w = K_MAP_SIZE;
    SrcR.h = yOff / pixelSize;

    DestR.x = 0;
    DestR.y = sz(K_DISPLAY_SIZE) - yOff;
    DestR.w = sz(K_DISPLAY_SIZE) - xOff;
    DestR.h = yOff;

    SDL_RenderCopy(renderer, texture, &SrcR, &DestR);

}


