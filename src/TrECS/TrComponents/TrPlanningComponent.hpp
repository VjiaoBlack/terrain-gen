#pragma once

/**
 * TrComponents.hpp
 */

#include <set>

#include "../TrComponents.hpp"

class TrEntity;

using namespace std;

/**
 * @brief holds information about a intended destination to go to
 * @details urgency is on the scale of 0 to 7
 */
struct tr_target_t {
  SDL_Point m_loc;
  uint8_t urgency;
};

/**
 * @brief interface for all planning components
 */
class TrPlanningComponent : public TrComponent {
 public:
  set<tr_target_t> m_targets;

  TrPlanningComponent() = default;
  ~TrPlanningComponent() override = default;

  /**
   * @brief based on the game and entity state, figure out something to do
   */
  void update(TrGame *game, TrEntity *entity) override;
};
