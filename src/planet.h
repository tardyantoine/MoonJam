#ifndef PLANETS_H
#define PLANETS_H

#include "SDL.h"
#include <vector>
#include "graphics.h"
#include "point.h"
#include "space_body.h"

class Planet : SpaceBody {
 public:
  Planet(Point pos, int r, int soi) : SpaceBody(pos, r) { mSoi = soi; }
  void draw(SDL_Renderer * renderer);
  Point computeAccelerationContribution(Point Pos);

 protected:
  // Constants
  const int kSoiRadiusFactor = 30;
  const int kGravity = 9000000;

  // Member variables
  int mSoi; // Radius of sphere of influence
};

#endif