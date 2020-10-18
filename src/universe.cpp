#ifndef UNIVERSE_CPP
#define UNIVERSE_CPP
#include <iostream>
#include "universe.h"

void Universe::run(uint planetNumber)
{
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* surfC = NULL;
    SDL_Texture* messageC = NULL;
    SDL_Surface* surfP = NULL;
    SDL_Texture* messageP = NULL;
    SDL_Surface* scoreSurf = NULL;
    SDL_Texture* scoreMessage = NULL;

    // Get resolution
    SDL_DisplayMode DM;
    SDL_GetDesktopDisplayMode(0, &DM);
    kWidth = DM.w;
    kHeight = DM.h;

    // Needs to happen after resolution is set
    createBodies(planetNumber);
    
    if (SDL_CreateWindowAndRenderer(kWidth, kHeight, 0, &window, &renderer) == 0) {
      // Score Rect
      SDL_Rect scoreRect; 
      scoreRect.h = 40;
      scoreRect.x = kWidth - 120;
      scoreRect.y = kHeight - 150; 

      // Instructions message
      TTF_Init();
      TTF_Font* nasa = TTF_OpenFont("../fonts/nasalization-rg.ttf", 500);
      SDL_Color white = {255, 255, 255};
      SDL_Rect messageRect; 
      messageRect.w = 1000;  
      messageRect.h = 40;
      messageRect.x = kWidth/2 - messageRect.w/2;
      messageRect.y = 10;
      surfP = TTF_RenderText_Solid(
        nasa, "UP/DOWN Arrow to control speed, ENTER to create Moon", white);
      messageP = SDL_CreateTextureFromSurface(renderer, surfP);
      surfC = TTF_RenderText_Solid(
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
        for (auto it = begin(mMoons); it != end(mMoons); ++it) {
          it->get()->draw(renderer);

          if(it->get()->getDeadBool()) { // Erase Moon if it collided or too far
            mMoons.erase(it);
            if(mMoons.empty()) {
              // Create new Moon
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
          else {
            it->get()->updateState(mPlanets, kWidth, kHeight);
          }
        }

        // Display instuctions message
        mState == FsmState::CreatingMoon ? 
          SDL_RenderCopy(renderer, messageC, NULL, &messageRect) : 
          SDL_RenderCopy(renderer, messageP, NULL, &messageRect);

        // Display score
        scoreSurf = TTF_RenderText_Solid(nasa, std::to_string(mMoons.size()).c_str(), white);
        scoreMessage = SDL_CreateTextureFromSurface(renderer, scoreSurf);
        scoreRect.w = 30 * (std::floor(mMoons.size() / 10) + 1); // Change box width for bigger numbers
        SDL_RenderCopy(renderer, scoreMessage, NULL, &scoreRect);

        // Draw init velocity vector
        if(mState == FsmState::CreatingMoon) {
          int mouseX = 0;
          int mouseY = 0;
          Point pos = mMoons.back()->getPos();
          SDL_GetMouseState(&mouseX, &mouseY);
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
          SDL_RenderDrawLine(renderer, mouseX, mouseY, pos.mX, pos.mY);
          drawCircle(renderer, Point(mouseX, mouseY), 10);
        }

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
    if(scoreSurf) { SDL_FreeSurface(scoreSurf); }
    if(scoreMessage) { SDL_DestroyTexture(scoreMessage); }
  }
  SDL_Quit();
}

void Universe::createBodies(uint planetNumber) {
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

Universe::Universe() {
  mState = FsmState::CreatingMoon;
}

#endif