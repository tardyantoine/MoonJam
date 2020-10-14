#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "SDL.h"
#include "graphics.h"

void drawCircle(SDL_Renderer * renderer, Point p, int radius) {
   const int diameter = (radius * 2);

   int x = (radius - 1);
   int y = 0;
   int tx = 1;
   int ty = 1;
   int error = (tx - diameter);

   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(renderer, p.mX + x, p.mY - y);
      SDL_RenderDrawPoint(renderer, p.mX + x, p.mY + y);
      SDL_RenderDrawPoint(renderer, p.mX - x, p.mY - y);
      SDL_RenderDrawPoint(renderer, p.mX - x, p.mY + y);
      SDL_RenderDrawPoint(renderer, p.mX + y, p.mY - x);
      SDL_RenderDrawPoint(renderer, p.mX + y, p.mY + x);
      SDL_RenderDrawPoint(renderer, p.mX - y, p.mY - x);
      SDL_RenderDrawPoint(renderer, p.mX - y, p.mY + x);

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

#endif