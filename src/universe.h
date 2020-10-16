#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "SDL.h"
#include "SDL_ttf.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <thread>
#include <memory>
#include "moon.h"
#include "planet.h"


enum FsmState {
  CreatingMoon,
  PilotingMoon
};

class Universe {
 public:
  Universe();
  void run(uint planetNumber); // Main simulation loop
  
 private:
  // Graphics
  int kHeight;
  int kWidth;
  const float kSpeedCoeff = 10;
  const float kAccel = 1.01;
  const float kDecel = 0.99;
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  // Data structures
  std::vector<std::shared_ptr<Moon>> mMoons;
  std::vector<std::thread> mThreads;
  std::vector<std::shared_ptr<Planet>> mPlanets;
  FsmState mState;

  // Methods
  void createBodies(uint planetNumber);
};

#endif