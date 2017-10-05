/*these functions have either been taken directly from
 NeillsSDLprogram or adpated from it*/
#include "extens.h"

void setWdwVaribles(Window *wdw)
{

  wdw->finished = 0;
  wdw->x = HALF(SIZE);
  wdw->y = HALF(SIZE);
  wdw->x1 = 0;
  wdw->y1 = 0;
  wdw->angle = NORTH;

}

void initialiseWindow(Window *wdw)
{

  if(SDL_Init(SDL_INIT_VIDEO) != 0 ){
    ERROR_SDL("Cannot Initialise SDL");
  }
  if((wdw->win = SDL_CreateWindow("Turtle Interpreter",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          SIZE, SIZE, SDL_WINDOW_SHOWN)) 
    == NULL){
    ERROR_SDL("Window can not be created");
  }
  if((wdw->renderer = SDL_CreateRenderer(wdw->win, -1, 0))
     == NULL){
    ERROR_SDL("Cannot Initialise SDL Renderer");
  }
  if(SDL_SetRenderDrawColor(wdw->renderer, BLACK) != 0){
    ERROR_SDL("Cannot set Colour");
  }

  if(SDL_RenderClear(wdw->renderer) != 0){
    ERROR_SDL("Cannot Set Renderer");
  }
  
  SDL_RenderPresent(wdw->renderer);

  if(SDL_SetRenderDrawColor(wdw->renderer, WHITE) != 0){
    ERROR_SDL("Cannot Set Draw Colour");
  }

}

void drawLine(Window *wdw, int x1, int y1, int x2, int y2)
{

  SDL_RenderDrawLine(wdw->renderer, x1, y1, x2, y2);

}

/* Gobble all events & ignore most 
   for full list of events - 
https://wiki.libsdl.org/SDL_EventType */
void Neill_SDL_Events(Window *sw)
{

  SDL_Event event;
  while(SDL_PollEvent(&event)) 
  {      
    switch (event.type){
      case SDL_QUIT:
      case SDL_MOUSEBUTTONDOWN:
      case SDL_KEYDOWN:
        sw->finished = 1;
    }
  }

}

void freeWin(Window *wdw)
{
  SDL_DestroyRenderer(wdw->renderer);
  SDL_DestroyWindow(wdw->win);
}


void move(Window *wdw, int a)
{
  find_x_y(wdw, a);
  drawLine(wdw, wdw->x, wdw->y, wdw->x1, wdw->y1);
  wdw->x = wdw->x1;
  wdw->y = wdw->y1;
  SDL_RenderPresent(wdw->renderer);
  SDL_UpdateWindowSurface(wdw->win);
}

void turnLeft(Window *wdw, int a)
{
  wdw->angle += (a * -1);
}

void turnRight(Window *wdw, int a)
{
  wdw->angle += a;
}







