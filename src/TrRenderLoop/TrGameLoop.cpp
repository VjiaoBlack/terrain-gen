/**
 * TrGameLoop.cpp
 */

#include "TrGameLoop.hpp"

void TrGameLoop::update(const TrGame* game) {
    game->m_map->update(game->m_keysDown);

}

void TrGameLoop::render(const TrGame* game) {
    renderTextureWithOffset(game->m_SDLRenderer, game->m_mapTexture, game->m_xOff, game->m_yOff,
                            game->c_pixelSize);


}
