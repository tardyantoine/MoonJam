#ifndef PLANETS_CPP
#define PLANETS_CPP

#include "planet.h"

#define A 9000000

void Planet::draw(SDL_Renderer * renderer) {
  // Draw planet
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  drawCircle(renderer, mP, mR);
  
  // Draw sphere of influence
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor(renderer, 150, 150, 150, 100);
  drawCircle(renderer, mP, mSoi);
}

Point Planet::computeAccelerationContribution(Point pos) {
  Point a = mP - pos;
  float r = std::sqrt(std::pow(a.mX,2) + std::pow(a.mY,2));

  // Compute component if within sphere of influence
  if(r <= mSoi) {
    a = a * (kGravity / std::pow(r, 2));  
  }
  else {
    a = {0, 0};
  } 
  return a;
}

#endif