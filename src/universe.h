#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "SDL.h"
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
  void run(); // Main simulation loop
  
 private:
  // Graphics
  const int kHeight = 1080;
  const int kWidth  = 1920;
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
};

#endif