#ifndef PLANETS_H
#define PLANETS_H

#include "SDL.h"
#include <vector>
#include "graphics.h"
#include "point.h"

class Planet {
 public:
  Planet(Point pos, int r) : mP(pos), mR(r) { mSoi = r * kSoiRadiusFactor; }
  void draw(SDL_Renderer * renderer);
  Point computeAccelerationContribution(Point Pos);

 protected:
  // Constants
  const int kSoiRadiusFactor = 30;
  const int kGravity = 9000000;

  Point mP;
  int mR;
  int mSoi; // Radius of sphere of influence
};

#endif