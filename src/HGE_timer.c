#include "SDL.h"
#include "HGE_mem.h"
#include "HGE_timer.h"


void
HGE_timer_init(void* ptr)
{
  HGE_timer_t* timer = ptr;

  timer->start_tick = 0;
  timer->length     = 0;
}

void*
HGE_timer_alloc(void)
{
  void* ptr;

  ptr = HGE_alloc(HGE_timer_t);
  if(ptr != NULL)
    HGE_timer_init(ptr);

  return ptr;
}

void
HGE_timer_free(void* ptr)
{
  if(ptr != NULL)
    HGE_free(ptr);
}

void
HGE_timer_set(HGE_timer_t* t, Uint32 length)
{
  t->length = length;
  t->start_tick = SDL_GetTicks();
}

void
HGE_timer_set_start(HGE_timer_t* t, Uint32 start_tick)
{
  t->start_tick = start_tick;
}

void
HGE_timer_set_end(HGE_timer_t* t, Uint32 end_tick)
{
  t->length = (end_tick - t->start_tick);
}

int
HGE_timer_done(HGE_timer_t* t)
{
  return (SDL_GetTicks() >= (t->start_tick + t->length));
}

Sint32
HGE_timer_time_left(HGE_timer_t* t)
{
  return ((t->start_tick + t->length) - SDL_GetTicks());
}

Sint32
HGE_timer_time_past(HGE_timer_t* t)
{
  return (SDL_GetTicks() - t->start_tick);
}

int
HGE_timer_done_given(HGE_timer_t* t, Uint32 start_tick)
{
  return (SDL_GetTicks() >= (start_tick + t->length));
}

Sint32
HGE_timer_time_left_given(HGE_timer_t* t, Uint32 start_tick)
{
  return ((start_tick + t->length) - SDL_GetTicks());
}

Sint32
HGE_timer_time_past_given(HGE_timer_t* t, Uint32 start_tick)
{
  return (SDL_GetTicks() - start_tick);
}
