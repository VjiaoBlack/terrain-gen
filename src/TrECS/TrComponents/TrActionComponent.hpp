#pragma once

/**
 * TrActionComponent.hpp
 */

#include <map>
#include "../TrComponents.hpp"

class TrFormula;
class TrItem;

using namespace std;

/**
 * @brief interface for all physics components
 */
class TrActionComponent : public TrComponent {
 public:
  // indicates the current action of the entity
  // note: currently, all possible actions are stored as formulas.
  TrFormula* m_action;
  map<TrItem *, int> m_progress;

  /**
   * @brief calculates the physics stuffs
   */
  template<class C>
  void update(TrGame *game, C *entity);
};

template<class C>
void TrActionComponent::update(TrGame *game, C *entity) {
  //
}
