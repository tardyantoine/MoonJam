#ifndef MOON_H
#define MOON_H

#include "SDL.h"
#include <vector>
#include "graphics.h"
#include "planet.h"
#include <queue>
#include <ctime>
#include <memory>

class Moon {
 public:
  Moon(Point p, Point v, int r, bool moving);
  void updateState(const std::vector<std::shared_ptr<Planet>> &planets);
  void drawMoon(SDL_Renderer * renderer);
  void initTail();
  void modifySpeed(float modifier);
  void setMoving(bool m);
  void setSpeed(Point v);
  Point getPos();
  
 private:
  clock_t lastUpdate;
  bool mMoving;
  Point mP; // Position
  Point mV; // Speed
  int mR; // Radius
  std::deque<Point> mTail;
  long mRed;
  long mGreen;
  long mBlue;

  void updateTail();
};

#endif