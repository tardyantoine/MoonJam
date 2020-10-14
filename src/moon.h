#ifndef MOON_H
#define MOON_H

#include "SDL.h"
#include <vector>
#include "graphics.h"

class Moon {
 public:
  Moon(Point p, Point v, int r);
  ~Moon();
  void updateState(const std::vector<Planet> &planets);
  void drawMoon(SDL_Renderer * renderer);
  void initTail();
  
 private:
  Point mP; // Position
  Point mV; // Speed
  int mR; // Radius
  std::deque<Point_t> mTail;

  void updateTail();
};

#endif