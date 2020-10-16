#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "SDL.h"
#include <vector>
#include "moon.h"
#include "planet.h"

enum FsmState {
  CreatingMoon,
  PilotingMoon
};

class Universe {
 public:
  Universe();
  // Universe(std::string map_csv_filename);
  ~Universe();
  void run(); // Main simulation loop
  
 private:
  // Graphics
  int const kHeight = 1080;
  int const kWidth  = 1920;
  float const kSpeedCoeff = 10;
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  // Data structures
  std::vector<Moon*> mMoons;
  std::vector<Planet*> mPlanets;
  FsmState mState;
  // void readCsvMap(std::string filename);
};

#endif