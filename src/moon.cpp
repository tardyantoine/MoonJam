#ifndef MOON_CPP
#define MOON_CPP

#include "SDL.h"
#include <vector>
#include "moon.h"
#include "graphics.h"
#include "point.h"
#include "planet.h"
#include <iostream>

#define TAIL_LENGTH 100000
#define TAIL_INTERVAL 400

Moon::Moon(Point p, Point v, int r) {
  mP = p;
  mV = v;
  mR = r;
  initTail();
}

void Moon::initTail() {
  for(int i = 0; i < TAIL_LENGTH; ++i) {
    mTail.push_back(mP);
  }
}

void Moon::updateTail() {
  mTail.pop_front();
  mTail.push_back(mP);
}

void Moon::updateState(const std::vector<Planet*> &planets) {
  Point a(0, 0);
  for(Planet* p : planets) {
    a = a + p->computeAccelerationContribution(mP);
  }

  mP = mP + mV;
  updateTail();
  mV = mV + a;
}

void Moon::modifySpeed(float modifier) {
  mV = mV * modifier;
}

void Moon::drawMoon(SDL_Renderer * renderer) {
  // Draw tail
  Point p(0, 0);
  float intensity = 0;
  for(int i = 0; i < TAIL_LENGTH; i += TAIL_INTERVAL) {
    p = mTail[i];
    float intensity = 255.0f * ((float)i / TAIL_LENGTH);
    SDL_SetRenderDrawColor(renderer, intensity, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, p.mX, p.mY);
  }

  // Draw actual Moon
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  drawCircle(renderer, mP, mR);
}

#endif