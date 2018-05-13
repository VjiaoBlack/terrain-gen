#pragma once
/**
 * TrGraphics.hpp
 *
 * Victor Jiao
 *
 * Stores wrappers for SDL2
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <memory>

#include "Perlin.hpp"
#include "Utils.hpp"

// https://swarminglogic.com/jotting/2015_05_smartwrappers
template<class T, class D = std::default_delete<T>>
struct shared_ptr_with_deleter : public std::shared_ptr<T> {
  explicit shared_ptr_with_deleter(T *t = nullptr)
      : std::shared_ptr<T>(t, D()) {}

  void reset(T *t = nullptr) {
    std::shared_ptr<T>::reset(t, D());
  }
};

struct SDL_Deleter {
  static int boop;
  void operator()(SDL_Surface *ptr) { if (ptr) SDL_FreeSurface(ptr); }
  void operator()(SDL_Texture *ptr) { if (ptr) SDL_DestroyTexture(ptr); }
  void operator()(SDL_Renderer *ptr) { if (ptr) SDL_DestroyRenderer(ptr); }
  void operator()(SDL_Window *ptr) { if (ptr) SDL_DestroyWindow(ptr); }
  void operator()(TTF_Font *ptr) { if (ptr) TTF_CloseFont(ptr); }
};


typedef shared_ptr_with_deleter<SDL_Surface, SDL_Deleter> sdl_surface_pt;
typedef shared_ptr_with_deleter<SDL_Texture, SDL_Deleter> sdl_texture_pt;
typedef shared_ptr_with_deleter<TTF_Font, SDL_Deleter> ttf_font_pt;

struct TrSDL {
  static sdl_texture_pt null_texture();

  static ttf_font_pt null_font();
};

struct TrRenderer {
  unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> m_renderer
      = unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>(nullptr, SDL_DestroyRenderer);

  TrRenderer() = default;
  explicit TrRenderer(SDL_Renderer *renderer) {
    m_renderer = unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)>(renderer, SDL_DestroyRenderer);
  }

  // implicit cast to SDL_Renderer
  operator SDL_Renderer *() const { return m_renderer.get(); }
};

struct TrWindow {
  unique_ptr<SDL_Window, void (*)(SDL_Window *)> m_renderer
      = unique_ptr<SDL_Window, void (*)(SDL_Window *)>(nullptr, SDL_DestroyWindow);

  TrWindow() = default;
  explicit TrWindow(SDL_Window *window) {
    m_renderer = unique_ptr<SDL_Window, void (*)(SDL_Window *)>(window, SDL_DestroyWindow);
  }

  // implicit cast to SDL_Window
  operator SDL_Window *() const { return m_renderer.get(); }
};

using namespace std;


