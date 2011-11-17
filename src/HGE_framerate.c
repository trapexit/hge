#include "SDL.h"
#include "HGE_framerate.h"

void 
HGE_framerate_init(HGE_framerate_t* f)
{
  f->mspf = (1000 / HGE_DEFAULT_FRAMERATE);
  f->last_tick = SDL_GetTicks();
}

void
HGE_framerate_set(HGE_framerate_t* f, int rate)
{
  f->mspf = (1000 / rate);
  f->last_tick = SDL_GetTicks();
}

int
HGE_framerate_get(HGE_framerate_t* f)
{
  return (1000 / f->mspf);
}

void
HGE_framerate_delay(HGE_framerate_t* f)
{
  int diff;
  
  diff = (f->mspf - (SDL_GetTicks() - f->last_tick));
  if(diff > 0)
    SDL_Delay((Uint32)diff);
  
  f->last_tick = SDL_GetTicks();
}
