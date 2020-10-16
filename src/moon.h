#ifndef MOON_H
#define MOON_H

#include "SDL.h"
#include <vector>
#include "graphics.h"
#include "planet.h"
#include "space_body.h"
#include <queue>
#include <ctime>
#include <memory>

class Moon : public SpaceBody{
 public:
  Moon(Point p, Point v, int r, bool moving);
  void updateState(const std::vector<std::shared_ptr<Planet>> &planets);
  void draw(SDL_Renderer * renderer);
  void initTail();
  void modifySpeed(float modifier);
  void setMoving(bool m);
  void setSpeed(Point v);
  Point getPos();
  
 private:
  // Constants
  const int kTailLength = 300;
  const int kTailInterval = 1;

  clock_t lastUpdate;
  bool mMoving;
  Point mV; // Speed
  std::deque<Point> mTail;
  long mRed;
  long mGreen;
  long mBlue;

  void updateTail();
};

#endif