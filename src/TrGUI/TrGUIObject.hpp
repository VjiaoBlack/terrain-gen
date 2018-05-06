#pragma once

/**
 * TrGUIObject.hpp
 * Generic class for all GUI objects
 */

#include "../TrGame.hpp"
#include "../TrGraphics.hpp"

class TrGUIObject {
 private:
 public:
  TrGame* m_game;
  bool m_activated;
  SDL_Texture* m_texture;
  SDL_Rect m_srcRect{};
  SDL_Rect m_destRect{};
  SDL_Rect m_rect{};

  TrGUIObject() : m_game(nullptr), m_activated(false), m_texture(nullptr){};
  explicit TrGUIObject(TrGame* game) : m_game(game){};
  virtual ~TrGUIObject() = default;;

  virtual void draw() = 0;
  virtual void update() = 0;
};