/** 
 * TrEntityFactory.cpp
 *  
 * Victor Jiao
 */

#include <TrECS/TrEntities/TrActorEntity.hpp>
#include <TrECS/TrComponents/TrGraphicsComponent.hpp>
#include <TrECS/TrComponents/TrPhysicsComponent.hpp>
#include <TrECS/TrComponents/TrPlanningComponent.hpp>
#include "TrEntityFactory.hpp"

/**
 * Sadly, CLion doesn't support constexpr.
 * Otherwise, you could implement this with just a constexpr.
 */
