/**
 * test_driving.cpp
 *
 * Victor Jiao
 *
 * a test file for testing driving.
 */

#include "TrGame.hpp"

int main(int argv, char *argc[]) {
  shared_ptr<TrGame> renderer = make_shared<TrGame>();
  renderer->run();
  renderer.reset();
  TTF_Quit();
  SDL_Quit();
  return 0;
}