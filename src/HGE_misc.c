#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_gl_funcs.h"
#include "HGE_mem.h"
#include "HGE_config.h"
#include "HGE_misc.h"

int
HGE_misc_rect_collision(SDL_Rect* r0, SDL_Rect* r1)
{
  if((r1->y > (r0->y + r0->h)) ||
     (r0->y > (r1->y + r1->h)) ||
     (r1->x > (r0->x + r0->w)) ||
     (r0->x > (r1->x + r1->w)))
    return 0;

  return 1;
}

void
HGE_take_screenshot(const char* path)
{
  int i;
  int w,h;
  char* pixels;
  SDL_Surface* surface;

  HGE_config_get_resolution(&w, &h);

  pixels = HGE_malloc(3 * w * h);
  if(pixels == NULL)
    return;

  surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
				 w, h, 24,
				 HGE_RMASK_8_8_8_8,
				 HGE_GMASK_8_8_8_8,
				 HGE_BMASK_8_8_8_8,
				 0);

  HGE_glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);  
  for(i = 0; i < h; i++)
    memcpy((((char*)surface->pixels) + surface->pitch * i),
	   (pixels + (3*w*(h-i-1))), (w*3));
  HGE_free(pixels);

  SDL_SaveBMP(surface, path);
  SDL_FreeSurface(surface);
}
