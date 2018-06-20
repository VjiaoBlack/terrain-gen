#pragma once

/**
 * TrEntities.hpp
 */

#include "../TrGame.hpp"
#include "TrComponents.hpp"

#include "TrComponents/TrGraphicsComponent.hpp"
#include "TrComponents/TrPhysicsComponent.hpp"
#include "TrComponents/TrPlanningComponent.hpp"

class TrGame;
//
///**
// * @brief General interface for all game entities
// */
//class TrEntity {
// public:
//  TrGame *m_game;
//
//  SDL_Point m_loc;
//
//  TrEntity(TrGame *game, SDL_Point xy) : m_game(game), m_loc(xy) {}
//  virtual ~TrEntity() {}; // TODO: make TrEntity uninstantiable
//};
