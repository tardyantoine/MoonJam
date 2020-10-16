#ifndef MOON_CPP
#define MOON_CPP

#include <iostream>
#include <ctime>
#include <vector>
#include "SDL.h"
#include "moon.h"
#include "graphics.h"
#include "point.h"
#include "planet.h"

#define TAIL_LENGTH 300
#define TAIL_INTERVAL 1

Moon::Moon(Point p, Point v, int r, bool moving) {
  mP = p;
  mV = v;
  mR = r;
  mMoving = moving;
  initTail();
  srand(clock());
  mRed = rand() % 255;
  mGreen = rand() % 255;
  mBlue = rand() % 255;
}

void Moon::setSpeed(Point v) {
  mV.mX = v.mX;
  mV.mY = v.mY;
}

void Moon::setMoving(bool m) {
  mMoving = m;
}

Point Moon::getPos() {
  return mP;
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

void Moon::updateState(const std::vector<std::shared_ptr<Planet>> &planets) {
  Point a(0, 0);
  
  if(mMoving) {
    for(auto p : planets) {
      a = a + p->computeAccelerationContribution(mP);
    }

    clock_t now = clock();
    if(lastUpdate == 0) { lastUpdate = now; }
    float dt = float(now - lastUpdate) / CLOCKS_PER_SEC;

    mV = mV + a * dt;
    mP = mP + mV * dt;
    updateTail();
  }

  lastUpdate = clock();
}

void Moon::modifySpeed(float modifier) {
  mV = mV * modifier;
}

void Moon::drawMoon(SDL_Renderer * renderer) {
  // Draw tail
  Point p(0, 0), pPrev(0, 0);
  float intensity = 0;
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  for(int i = 1; i < TAIL_LENGTH; i += TAIL_INTERVAL) {
    p = mTail[i];
    pPrev = mTail[i - 1];

    float intensity = 255.0f * ((float)i / TAIL_LENGTH);
    SDL_SetRenderDrawColor(renderer, mRed, mBlue, mGreen, intensity);

    SDL_RenderDrawLine(renderer, p.mX, p.mY, pPrev.mX, pPrev.mY);
  }

  // Draw actual Moon
  SDL_SetRenderDrawColor(renderer, mRed, mBlue, mGreen, SDL_ALPHA_OPAQUE);
  drawCircle(renderer, mP, mR);
}

#endif