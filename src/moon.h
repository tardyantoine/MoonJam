#ifndef MOON_H
#define MOON_H

#include "SDL.h"
#include <vector>
#include "graphics.h"
#include "planet.h"
#include <queue>

class Moon {
 public:
  Moon(Point p, Point v, int r);
  void updateState(const std::vector<Planet*> &planets);
  void drawMoon(SDL_Renderer * renderer);
  void initTail();
  void modifySpeed(float modifier);
  
 private:
  Point mP; // Position
  Point mV; // Speed
  int mR; // Radius
  std::deque<Point> mTail;

  void updateTail();
};

#endif