#include "SDL.h"
#include "HGE_mutex.h"

void*
HGE_mutex_alloc(void)
{
  return SDL_CreateMutex();
}

void
HGE_mutex_free(void* ptr)
{
  if(ptr != NULL)
    SDL_DestroyMutex(ptr);
}
