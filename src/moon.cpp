#ifndef MOON_CPP
#define MOON_CPP

#include "SDL.h"
#include <vector>
#include "moon.h"
#include "graphics.h"
#include "point.h"
#include "planet.h"

#define TAIL_LENGTH 100000
#define TAIL_INTERVAL 400

Moon::Moon(Point p, Point v, int r) {
  mP = p;
  mV = v;
  mR = r;
  initTail();
}


~Moon::Moon() {
}

void Moon::initTail() {
for(int i = 0; i < TAIL_LENGTH; ++i) {
  tail.push_back(mP);
}

void updateTail() {
  tail.pop_front();
  tail.push_back(mP);
}

void Moon::updateState(const std::vector<Planet> &planets) {
  Point a = 0;
  for(Planet p : planets) {
    a += p->computeAcceleration(mP);
  }

  mP = mP + mV;
  updateTail();
  mV = mV + a;
}

void Moon::drawMoon(SDL_Renderer * renderer) {
  // Draw tail
  Point p;
  float intensity = 0;
  for(int i = 0; i < TAIL_LENGTH; i += TAIL_INTERVAL) {
    p = tail[i];
    float intensity = 255.0f * ((float)i / TAIL_LENGTH);
    SDL_SetRenderDrawColor(renderer, intensity, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, p.mX, p.my);
  }

  // Draw actual Moon
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  Graphics::drawCircle(renderer, mP, mR);
}

#endif