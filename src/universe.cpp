#ifndef UNIVERSE_CPP
#define UNIVERSE_CPP

#include <iostream>       // std::cin, std::cout
#include <queue>          // std::queue
#include "SDL.h"
#include "universe.h"
#include "planet.h"
#include "moon.h"

void Universe::run()
{
  Planet* p = new Planet(Point(kWidth/2, kHeight/2), 50);
  mPlanets.push_back(p);
  std::cout << "Planets created..." << std::endl;

  Moon* m = new Moon(Point(kWidth/2, kHeight/4), Point(0.06, 0.0), 5);
  mMoons.push_back(m);
  std::cout << "Moons created..." << std::endl;

  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_CreateWindowAndRenderer(kWidth, kHeight, 0, &window, &renderer) == 0) {
      
      // Infinite MAIN loop
      SDL_bool done = SDL_FALSE;
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

        case SDL_KEYDOWN:
          switch( event.key.keysym.sym ){
          case SDLK_UP:
            // TODO update
            mMoons[0]->modifySpeed(1.00001);
            break;
          case SDLK_DOWN:
            mMoons[0]->modifySpeed(0.95);
            break;
          default:
            break;
          break;
          }
        }
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