/**
 * TrGraphics
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include "TrGraphics.hpp"


// hack inverse square root copied from Doom
float Q_rsqrt(float number) {
    return 1.0f / sqrt(number);
}


inline uint32_t multiplyColor(uint32_t color, double r, double g, double b) {
    int cr = (color & K_R_MASK) >> 16;
    int cg = (color & K_G_MASK) >> 8;
    int cb = color & K_B_MASK;

    cr = floor(cr * r);
    cg = floor(cg * g);
    cb = floor(cb * b);

    if (cr > 255) cr = 255;
    if (cg > 255) cg = 255;
    if (cb > 255) cb = 255;

    if (cr < 0) cr = 0;
    if (cg < 0) cg = 0;
    if (cb < 0) cb = 0;

    uint32_t ret = K_A_MASK;
    ret |= cr << 16;
    ret |= cg << 8;
    ret |= cb;

    return ret;
}

inline uint32_t shiftColor(uint32_t color, int r, int g, int b) {
    int cr = (color & K_R_MASK) >> 16;
    int cg = (color & K_G_MASK) >> 8;
    int cb = color & K_B_MASK;

    cr += r;
    cg += g;
    cb += b;

    if (cr > 255) cr = 255;
    if (cg > 255) cg = 255;
    if (cb > 255) cb = 255;

    if (cr < 0) cr = 0;
    if (cg < 0) cg = 0;
    if (cb < 0) cb = 0;

    uint32_t ret = K_A_MASK;
    ret |= cr << 16;
    ret |= cg << 8;
    ret |= cb;

    return ret;
}



void TrMap::updateColors() {
    if (!m_useMoisture) {
        int threshold[9] = {70, 100, 117, 132, 148, 165, 180, 198, 218};

        // uint32_t colors[9] = {0xFF1A2B56,0xFF253C78,0xFF3A5BAA,
        //                       0xFFEEDDBB,0xFF77BC49,0xFF58A327,
        //                       0xFF28771F,0xFF210E04,0xFF5B3F31};

        // no initial water colors,just land.
        uint32_t colors[9] = {0xFF664433,0xFFBB8866,0xFFEEDDBB,
                              0xFFEEDDBB,0xFF77BC49,0xFF58A327,
                              0xFF28771F,0xFF210E04,0xFF5B3F31};

        vec3 light(0.4, 0.2, 0.8);
        light.normalize();

        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                // render land
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

                // render water
                if (m_water->at(i,j) > 0.001) {
                    float height = m_water->at(i,j) + m_height->at(i,j);
                    m_diffuse->at(i,j) = 0xFF305090;

                    int rip = floor(height * 128.0) - 32;
                    m_diffuse->at(i,j) = shiftColor(m_diffuse->at(i,j), rip, rip, rip); 
                } else {
                    float wat = 0.6 * light.dot(m_normal->at(i,j));
                    m_diffuse->at(i,j) = multiplyColor(m_diffuse->at(i,j), wat + 0.4, wat + 0.4, wat + 0.4);

                }


            }
        }
    } else {

        // no initial water colors,just land.
        uint32_t c_wetdirt = 0xFF664433;
        uint32_t c_drydirt = 0xFFBB8866;
        uint32_t c_sand = 0xFFEEDDBB;
        uint32_t c_grass = 0xFF67AC40;
        uint32_t c_forest = 0xFF58A327;
        uint32_t c_rainforest = 0xFF28771F;
        uint32_t c_rock = 0xFFAB9F91;

        vec3 light(0.4, 0.2, 0.8);
        light.normalize();

        for (int i = 0; i < m_rows; i++) {
            for (int j = 0; j < m_cols; j++) {
                // errr how do I do this....
                if (m_height->at(i,j) > 0.7) {
                    m_diffuse->at(i,j) = c_rock;
                } else {
                    // TODO: LERPING

                    if (m_moisture->at(i,j) > 0.05) {
                        m_diffuse->at(i,j) = c_wetdirt;
                    } else {
                        m_diffuse->at(i,j) = c_drydirt;
                    }

                    if (m_vegetation->at(i,j) > 0.5) {
                        m_diffuse->at(i,j) = c_rainforest;
                    }
                }

                // render water
                if (m_water->at(i,j) > 0.001) {
                    float height = m_water->at(i,j) + m_height->at(i,j);
                    m_diffuse->at(i,j) = 0xFF305090;

                    int rip = floor(height * 128.0) - 32;
                    m_diffuse->at(i,j) = shiftColor(m_diffuse->at(i,j), rip, rip, rip); 
                } else {
                    float wat = 0.6 * light.dot(m_normal->at(i,j));
                    m_diffuse->at(i,j) = multiplyColor(m_diffuse->at(i,j), wat + 0.4, wat + 0.4, wat + 0.4);

                }


            }
        }
        
    }
    
}

void TrMap::updateMoisture() {
    // okay set moisture to 1 if there is some water
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            if (m_water->at(i,j) > 0.002) {
                m_moisture->at(i,j) = 1.0;
            } else {
                m_moisture->at(i,j) = m_moisture->at(i,j) * 0.95;
            }

            if (m_moisture->at(i,j) > 0.1 && m_moisture->at(i,j) < 0.5) {
                m_vegetation->at(i,j) += 0.01;
            } else {
                m_vegetation->at(i,j) -= 0.01;
            }
        }
    }

    m_moisture->boxBlur();
}

void TrMap::updateWater(bool erosion) {
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            double m = m_height->get(i,j) +
                       m_water->get(i,j);
            int mi = i;
            int mj = j;
            double mh = m;

            // straight
            if (m_height->get(i+1,j) + m_water->get(i+1,j) < mh) {
                mi = i+1; mj = j;
                mh = m_height->get(i+1,j) + m_water->get(i+1,j); }

            if (m_height->get(i-1,j) + m_water->get(i-1,j) < mh) {
                mi = i-1; mj = j;
                mh = m_height->get(i-1,j) + m_water->get(i-1,j); }

            if (m_height->get(i,j+1) + m_water->get(i,j+1) < mh) {
                mi = i; mj = j+1;
                mh = m_height->get(i,j+1) + m_water->get(i,j+1); }

            if (m_height->get(i,j-1) + m_water->get(i,j-1) < mh) {
                mi = i; mj = j-1;
                mh = m_height->get(i,j-1) + m_water->get(i,j-1); }

            // diagonal
            if (m - (m_height->get(i+1,j+1) + m_water->get(i+1,j+1)) > (m - mh) * 1.4) {
                mi = i+1; mj = j+1;
                mh = m_height->get(i+1,j+1) + m_water->get(i+1,j+1); }

            if (m - (m_height->get(i-1,j-1) + m_water->get(i-1,j-1)) > (m - mh) * 1.4) {
                mi = i-1; mj = j-1;
                mh = m_height->get(i-1,j-1) + m_water->get(i-1,j-1); }

            if (m - (m_height->get(i+1,j-1) + m_water->get(i+1,j-1)) > (m - mh) * 1.4) {
                mi = i+1; mj = j-1;
                mh = m_height->get(i+1,j-1) + m_water->get(i+1,j-1); }

            if (m - (m_height->get(i-1,j+1) + m_water->get(i-1,j+1)) > (m - mh) * 1.4) {
                mi = i-1; mj = j+1;
                mh = m_height->get(i-1,j+1) + m_water->get(i-1,j+1); }


            
            

            if (mh < m - 0.0001) {
                double diff = m-mh;
                diff *= 0.5;

                // prevents water from cutting too deep
                diff *= 1.0 - m_water->get(i,j);
                if (diff > m_water->get(i,j)) {
                    diff = m_water->get(i,j);
                }

                m_water_temp->at(mi,mj) += diff;
                m_water_temp->at(i,j) -= diff;
            }
        }
    }
    // wait or shud we blur the moisture map?

    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {

            if (erosion) {
                double change = m_water_temp->at(i,j);
                if (change > 0.0) {
                    // adding land
                    // TODO WHAT DO I DO WITH THIS
                    change = change * 0.5;
                }
                change *= abs(change * 40.0);
                m_height->at(i  ,j  ) += change /  8.0f;
                m_height->at(i+1,j+1) += change / 22.0f;
                m_height->at(i+1,j  ) += change / 16.0f;
                m_height->at(i+1,j-1) += change / 22.0f;
                m_height->at(i  ,j+1) += change / 16.0f;
                m_height->at(i  ,j-1) += change / 16.0f;
                m_height->at(i-1,j+1) += change / 22.0f;
                m_height->at(i-1,j  ) += change / 16.0f;
                m_height->at(i-1,j-1) += change / 22.0f;
            }
            m_water->at(i,j) += m_water_temp->at(i,j) -.00001;

            if (m_water->at(i,j) < 0.0) {
                m_water->at(i,j) = 0.0;
            } 
            if (m_water->at(i,j) > 1.0) {
                m_water->at(i,j) = 1.0;
            }

            m_water_temp->at(i,j) = 0.0;
        }
    }
}

void TrMap::updateNormals() {
    // go through the map
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            // l->r
            vec3 lr(2.0, 0.0, 255.0 * (m_height->at(i,j+1) - m_height->at(i,j-1)));
            // u->d
            vec3 ud(0.0, 2.0, 255.0 * (m_height->at(i+1,j) - m_height->at(i-1,j)));
            
            m_normal->at(i,j).x = lr.y * ud.z - lr.z * ud.y;
            m_normal->at(i,j).y = lr.z * ud.x - lr.x * ud.z;
            m_normal->at(i,j).z = lr.x * ud.y - lr.y * ud.x;

            m_normal->at(i,j).normalize();
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


