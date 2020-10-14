#ifndef PLANETS_H
#define PLANETS_H

#include "SDL.h"
#include <vector>
#include "graphics.h"
#include "point.h"

class Planet {
 public:
  Planet(Point pos, int r) : mPos(pos), mR(r) {}
  void drawPlanet(SDL_Renderer * renderer);
  Point computeAccelerationContribution(Point Pos);

 private:
  Point mPos;
  int mR;
};

#endif