#ifndef PLANETS_CPP
#define PLANETS_CPP

#include "SDL.h"
#include <vector>
#include "planet.h"
#include "graphics.h"
#include "point.h"
#include <iostream>

#define A 0.003

void Planet::drawPlanet(SDL_Renderer * renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  drawCircle(renderer, mPos, mR);
}

Point Planet::computeAccelerationContribution(Point pos) {
  Point a = mPos - pos;
  float r2 = std::pow(std::sqrt(std::pow(a.mX,2) + std::pow(a.mY,2)), 2);
  a = a * (A / r2);   
  return a;
}

#endif