#ifndef UNIVERSE_CPP
#define UNIVERSE_CPP
#include <iostream>
#include "universe.h"

// TODO:
// - Collisions
// Score and instructions
// - README!

void Universe::run()
{
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
        for(auto p : mPlanets) {
          p->draw(renderer);
        }

        // Draw moons and move them
        for(auto m : mMoons) {
          m->draw(renderer);
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
              mMoons.back()->modifySpeed(kAccel);
            }
            break;

          case SDLK_DOWN:
            if(mState == FsmState::PilotingMoon) {
              mMoons.back()->modifySpeed(kDecel);
            }
            break;

          case SDLK_RETURN:
            // Create new Moon
            if(mState == FsmState::PilotingMoon) {
              mState = FsmState::CreatingMoon;
              srand(clock());
              float newX = (float(rand() % kWidth) * 0.9f) + (0.05f * float(kWidth));
              float newY = (float(rand() % kHeight) * 0.9f) + (0.05f * float(kHeight));
              auto m = std::make_shared<Moon>(Point(newX, newY), Point(0.0, 0.0), 5, false);
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

Universe::Universe(uint planetNumber) {
  mState = FsmState::CreatingMoon;

  switch(planetNumber) {
  default:
  case 1:
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/2, kHeight/2), 50, 5000));
    break;

  case 2:
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/4,   kHeight/2), 50, 500));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/4*3, kHeight/2), 50, 500));
    break;

  case 3:
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/4,   kHeight/3),   30, 500));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/4*3, kHeight/3),   30, 500));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/2,   kHeight/3*2), 30, 500));
    break;

  case 4:
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/5,   kHeight/3),   20, 400));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/5*2, kHeight/3*2), 20, 400));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/5*3, kHeight/3),   20, 400));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/5*4, kHeight/3*2), 20, 400));
    break;

  case 5:
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/6,   kHeight/3),   20, 400));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/6*3, kHeight/3),   20, 400));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/6*5, kHeight/3),   20, 400));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/4,   kHeight/3*2), 20, 400));
    mPlanets.push_back(std::make_shared<Planet>(Point(kWidth/4*3, kHeight/3*2), 20, 400));
    break;
  }
  std::cout << "Universe created..." << std::endl;

  srand(clock());
  float newX = (float(rand() % kWidth) * 0.9f) + (0.05f * float(kWidth));
  float newY = (float(rand() % kHeight) * 0.9f) + (0.05f * float(kHeight));
  auto m = std::make_shared<Moon>(Point(newX, newY), Point(0.0, 0.0), 5, false);
  mMoons.push_back(m);
  std::cout << "First Moon created..." << std::endl;
}

#endif