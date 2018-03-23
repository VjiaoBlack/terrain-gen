#pragma once

/**
 * TrGUIObject.hpp
 * Generic class for all GUI objects
 */

#include "../TrGraphics.hpp"

class TrGame;

class TrGUIObject {
 private:
 public:
  TrGame* m_game;

  TrGUIObject() : m_game(nullptr){};
  TrGUIObject(TrGame* game) : m_game(game){};
  virtual ~TrGUIObject(){};

  void draw();
  void update();
};