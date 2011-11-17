#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_parallax.h"

void
HGE_parallax_init(HGE_parallax_t* p)
{
  p->img          = NULL;
  p->width        = 0;
  p->height       = 1;
  p->x            = 0.0;
  p->y            = 0.0;
  p->px           = 0.0;
  p->py           = 0.0;
  p->dx           = 0.0;
  p->time_to_wait = 0.0;
  p->auto_scroll  = HGE_FALSE;
}




