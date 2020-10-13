#include <iostream>       // std::cin, std::cout
#include <queue>          // std::queue
#include "SDL.h"

#define WINDOW_HEIGHT 1080
#define WINDOW_WIDTH 1920
#define MOON_Y WINDOW_HEIGHT/2
#define MOON_X WINDOW_WIDTH/2
#define MOON_RADIUS 5
#define A 0.003
#define TAIL_LENGTH 100000
#define TAIL_INTERVAL 400

float px;
float py;
float vx;
float vy;
float ax;
float ay;

struct Point_t {
  float x;
  float y;
};

std::deque<Point_t> tail;

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
   const int32_t diameter = (radius * 2);

   int32_t x = (radius - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
      SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
      SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void initTail(float x, float y) {
  Point_t p = {x, y};
  for(int i = 0; i < TAIL_LENGTH; ++i) {
    tail.push_back(p);
  }
}

void drawTail(SDL_Renderer* renderer) {
  Point_t p;
  float intensity = 0;
  for(int i = 0; i < TAIL_LENGTH; i += TAIL_INTERVAL) {
    p = tail[i];
    float intensity = 255.0f * ((float)i / TAIL_LENGTH);
    SDL_SetRenderDrawColor(renderer, intensity, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(renderer, p.x, p.y);
  }
}

void updateTail(float x, float y) {
  Point_t p = {x, y};
  tail.pop_front();
  tail.push_back(p);
}

void display(SDL_Renderer* renderer) {
  if(renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    
    drawTail(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    DrawCircle(renderer, MOON_X, MOON_Y, 50);
    DrawCircle(renderer, px, py, MOON_RADIUS);

    SDL_RenderPresent(renderer);
  }
}

void computeAcceleration() {
  float dx = MOON_X - px;
  float dy = MOON_Y - py;
  float r2 = std::pow(std::sqrt(std::pow(dx,2) + std::pow(dy,2)), 2);
  ax = dx * A / r2;
  ay = dy * A / r2; 
}

int main(int argc, char* argv[])
{
  px = MOON_X;
  py = MOON_Y/2;
  vx = 0.06;
  vy = 0.0;
  initTail(px, py);

  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {
      
      // Infinite MAIN loop
      SDL_bool done = SDL_FALSE;
      while(!done) {
        SDL_Event event;
        display(renderer);
        computeAcceleration();
        
        px += vx;
        py += vy;
        updateTail(px, py);
        vx += ax;
        vy += ay;

        SDL_PollEvent(&event);
        switch(event.type) {
        case SDL_QUIT:
          done = SDL_TRUE;
          break;

        case SDL_KEYDOWN:
          switch( event.key.keysym.sym ){
          case SDLK_UP:
              vx += 0.00001;
              vy += 0.00001;
              break;
          case SDLK_DOWN:
              vx -= 0.00001;
              vy -= 0.00001;
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
  return 0;
}