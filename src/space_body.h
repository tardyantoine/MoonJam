#ifndef SPACE_BODY_H
#define SPACE_BODY_H

#include "point.h"

class SpaceBody {
 public:
  SpaceBody(Point pos, int r) : mP(pos), mR(r) {}
  virtual void draw(SDL_Renderer * renderer) = 0;

 protected:
  Point mP;
  int mR;
};

#endif