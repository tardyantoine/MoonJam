#ifndef UNIVERSE_CPP
#define UNIVERSE_CPP
#include <iostream>
#include "universe.h"

// TODO:
// - Collisions
// - Score
// - README!

void Universe::run()
{
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* surfC = NULL;
    SDL_Texture* messageC = NULL;
    SDL_Surface* surfP = NULL;
    SDL_Texture* messageP = NULL;
    
    if (SDL_CreateWindowAndRenderer(kWidth, kHeight, 0, &window, &renderer) == 0) {
      
      // Instructions message
      TTF_Init();
      TTF_Font* nasa = TTF_OpenFont("../fonts/nasalization-rg.ttf", 500);
      SDL_Color white = {255, 255, 255};
      SDL_Rect messageRect; 
      messageRect.w = 1000;  
      messageRect.h = 40;
      messageRect.x = kWidth/2 - messageRect.w/2;
      messageRect.y = 10; 
      SDL_Surface* surfP = TTF_RenderText_Solid(
        nasa, "UP/DOWN arrow to control speed, ENTER to create Moon", white);
      messageP = SDL_CreateTextureFromSurface(renderer, surfP);
      SDL_Surface* surfC = TTF_RenderText_Solid(
        nasa, "LEFT click to define the new Moon's initial velocity vector", white);
      messageC = SDL_CreateTextureFromSurface(renderer, surfC);
                  
      // Main infinite loop
      SDL_bool done = SDL_FALSE;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      while(!done) {
        // Init
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

        // Display instuctions message
        mState == FsmState::CreatingMoon ? 
          SDL_RenderCopy(renderer, messageC, NULL, &messageRect) : 
          SDL_RenderCopy(renderer, messageP, NULL, &messageRect);

        // Render all
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

    if(renderer) { SDL_DestroyRenderer(renderer); }
    if(window) { SDL_DestroyWindow(window); }
    if(surfP) { SDL_FreeSurface(surfP); }
    if(messageP) { SDL_DestroyTexture(messageP); }
    if(surfC) { SDL_FreeSurface(surfC); }
    if(messageC) { SDL_DestroyTexture(messageC); }
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