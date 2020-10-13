#include "SDL.h"

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640
#define MOON_RADIUS 5

float px;
float py;
float vx;
float vy;
float ax;
float ay;

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

void draw(SDL_Renderer* renderer) {
  if(renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    DrawCircle(renderer, WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 50);
    DrawCircle(renderer, px, py, MOON_RADIUS);

    SDL_RenderPresent(renderer);
  }
}

int main(int argc, char* argv[])
{
  vx = 0.01;
  vy = 0.02;

  ax = 0.00001;
  ay = 0.00000;

  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer) == 0) {
      
      // Infinite MAIN loop
      SDL_bool done = SDL_FALSE;
      while(!done) {
        SDL_Event event;
        draw(renderer);

        px += vx;
        py += vy;

        vx += ax;
        vy += ay;



        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            done = SDL_TRUE;
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