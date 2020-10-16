#ifndef UNIVERSE_CPP
#define UNIVERSE_CPP

#include <iostream>
#include <queue>
#include <ctime>
#include <thread>
#include "SDL.h"
#include "universe.h"
#include "planet.h"
#include "moon.h"

// TODO:
// - multi thread
// - User input
// - random start point
// - Inherit moon planet
// - SOI
// - Braking
// - Collisions
// - Different worlds vs random
// - File map
// - Start menu buttons

#define FPS 2.0f

void Universe::run()
{
  Planet* p = new Planet(Point(kWidth/2, kHeight/2), 50);
  mPlanets.push_back(p);
  std::cout << "First Planet created..." << std::endl;

  srand(clock());
  float newX = (float(rand() % kWidth) * 0.9f) + (0.05f * float(kWidth));
  float newY = (float(rand() % kHeight) * 0.9f) + (0.05f * float(kHeight));
  Moon* m = new Moon(Point(newX, newY), Point(0.0, 0.0), 5, false);
  mMoons.push_back(m);
  std::cout << "First Moon created..." << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_CreateWindowAndRenderer(kWidth, kHeight, 0, &window, &renderer) == 0) {
      
      // Infinite MAIN loop
      SDL_bool done = SDL_FALSE;

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      while(!done) {
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        // Draw planets
        for(Planet* p : mPlanets) {
          p->drawPlanet(renderer);
        }

        // Draw moons and move them
        for(Moon* m : mMoons) {
          m->drawMoon(renderer);
          m->updateState(mPlanets);
        }
        SDL_RenderPresent(renderer);

        // Check for user input
        SDL_PollEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
          done = SDL_TRUE;
          break;

        case SDL_MOUSEBUTTONDOWN:
          if(mState == FsmState::CreatingMoon) {
            mState = FsmState::PilotingMoon;
            int mouseX = 0;
            int mouseY = 0;
            SDL_GetMouseState(&mouseX, &mouseY);
            Point pointer = {float(mouseX), float(mouseY)};
            Point newSpeed = pointer - mMoons.back()->getPos();
            mMoons.back()->setSpeed(newSpeed * kSpeedCoeff);
            mMoons.back()->setMoving(true);
            std::cout << "New Moon launched..." << std::endl;
          }
          break;

        case SDL_KEYDOWN:
          switch( event.key.keysym.sym ){
          case SDLK_UP:
            if(mState == FsmState::PilotingMoon) {
              mMoons.back()->modifySpeed(1.003);
            }
            break;

          case SDLK_DOWN:
            if(mState == FsmState::PilotingMoon) {
              mMoons.back()->modifySpeed(0.998);
            }
            break;

          case SDLK_RETURN:
            // Create new Moon
            if(mState == FsmState::PilotingMoon) {
              mState = FsmState::CreatingMoon;
              srand(clock());
              float newX = (float(rand() % kWidth) * 0.9f) + (0.05f * float(kWidth));
              float newY = (float(rand() % kHeight) * 0.9f) + (0.05f * float(kHeight));
              Moon* m = new Moon(Point(newX, newY), Point(0.0, 0.0), 5, false);
              mMoons.push_back(m);
              std::cout << "New Moon created..." << std::endl;
            }
            break;
          }
          break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    }

    if (renderer) {
      SDL_DestroyRenderer(renderer);
    }
    if (window) {
      SDL_DestroyWindow(window);
    }
  }
  SDL_Quit();
}

Universe::Universe() {
  mState = FsmState::CreatingMoon;
}

Universe::~Universe() {
  // Delete everything
  for(Planet* p : mPlanets) {
    delete(p);
  }

  // Draw moons and move them
  for(Moon* m : mMoons) {
    delete(m);
  }
}

#endif