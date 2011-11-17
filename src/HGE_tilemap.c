#include <stdlib.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_types.h"
#include "HGE_defines.h"
#include "HGE_mem.h"
#include "HGE_gl_funcs.h"
#include "HGE_math.h"
#include "HGE_image.h"
#include "HGE_primitives.h"
#include "HGE_message.h"
#include "HGE_view.h"
#include "HGE_tilemap.h"
#include "HGE_trash_stack.h"

HGE_typeinfo_t HGE_tilemap[] = {{sizeof(HGE_tilemap_t),
				 HGE_tilemap_init,
				 HGE_tilemap_clean,
				 NULL,
				 NULL}};

HGE_error_t
HGE_tilemap_init(void* ptr)
{
  HGE_tilemap_t* map = ptr;

  map->x               = 0.0f;
  map->y               = 0.0f;
  map->map_x           = 0;
  map->map_y           = 0;
  map->tile_x          = 0;
  map->tile_y          = 0;
  map->x_off           = 0;
  map->y_off           = 0;
  map->pixel_draw_w    = 0;
  map->pixel_draw_h    = 0;
  map->tiles_wide      = 0;
  map->tiles_high      = 0;
  map->tile_size       = 0.0f;
  map->tile_size_mask  = 0;
  map->tile_size_shift = 0;
  map->map             = NULL;

  return HGE_SUCCESS;
}

void* 
HGE_tilemap_alloc(void)
{
  void* ptr;

  ptr = HGE_alloc(HGE_tilemap_t);
  if(ptr != NULL)
    HGE_tilemap_init(ptr);

  return ptr;
}

void
HGE_tilemap_free(void* ptr)
{
  HGE_tilemap_t* tmap = ptr;

  if(tmap != NULL)
    {
      if(tmap->map != NULL)
	HGE_free(tmap->map);
      HGE_free(tmap);
    }
}

void
HGE_tilemap_clean(void* ptr)
{
  HGE_tilemap_t* tmap = ptr;

  if(tmap != NULL)
    {
      if(tmap->map != NULL)
	HGE_free(tmap->map);
    }
}

void
HGE_tilemap_set_map_size(HGE_tilemap_t* map, Uint32 w, Uint32 h)
{
  map->map = HGE_realloc(map->map, HGE_image_t*, (w * h));
  map->tiles_wide = w;
  map->tiles_high = h;
}

void
HGE_tilemap_get_map_size(HGE_tilemap_t* map, Uint32* w, Uint32* h)
{
  if(w != NULL)
    *w = map->tiles_wide;
  if(h != NULL)
    *h = map->tiles_high;
}

void
HGE_tilemap_set_tile(HGE_tilemap_t* map, Uint32 w, Uint32 h, HGE_image_t* image)
{
  map->map[((map->tiles_wide * h) + w)] = image;
}

HGE_image_t*
HGE_tilemap_get_tile(HGE_tilemap_t* map, Uint32 w, Uint32 h)
{
  return map->map[((map->tiles_wide * h) + w)];
}

void
HGE_tilemap_set_display_size(HGE_tilemap_t* map, Uint32 w, Uint32 h)
{
  map->pixel_draw_w = (float)(w * map->tile_size);
  map->pixel_draw_h = (float)(h * map->tile_size);
}

void
HGE_tilemap_set_tile_size(HGE_tilemap_t* map, Uint32 size)
{
  map->tile_size       = (float)size;
  map->tile_size_mask  = size - 1;
  map->tile_size_shift = HGE_math_shift_value(size);
}

void
HGE_tilemap_set_coord(HGE_tilemap_t* map, Uint32 x, Uint32 y)
{
  if((x + map->pixel_draw_w) < (map->tile_size * map->tiles_wide))
    {
      map->map_x  = x;
      map->tile_x = (x >> map->tile_size_shift);
      map->x_off  = -(x & map->tile_size_mask);
    }

  if((y + map->pixel_draw_h) < (map->tile_size * map->tiles_high))
    {
      map->map_y  = y;
      map->tile_y = (y >> map->tile_size_shift);
      map->y_off  = -(y & map->tile_size_mask);
    }
}

void
HGE_tilemap_inc_coord(HGE_tilemap_t* map, Sint32 x, Sint32 y)
{
  HGE_tilemap_set_coord(map, (map->map_x + x), (map->map_y + y));
}

void
HGE_tilemap_draw1(HGE_tilemap_t* tilemap)
{
  float x,y;
  int ty;
  HGE_view_t view;
  HGE_image_t** tile;

  HGE_glPushMatrix();

  HGE_glTranslatef(tilemap->x, tilemap->y, 0.0f);

  HGE_glGetIntegerv(GL_SCISSOR_BOX, (int*)&view);

  HGE_view_clip((int)tilemap->x,
		(int)tilemap->y,
		(int)tilemap->pixel_draw_w,
		(int)tilemap->pixel_draw_h);
  
  ty = tilemap->tile_y;

  y = (float)tilemap->y_off;
  while(y < tilemap->pixel_draw_h)
    {
      tile = &tilemap->map[((tilemap->tiles_wide * ty) + tilemap->tile_x)];
      
      x = (float)tilemap->x_off;
      while(x < tilemap->pixel_draw_w)
	{
	  if(*tile)
	    HGE_image_draw(*tile, x, y);

	  tile++;
	  x += tilemap->tile_size;
	}

      ty++;		       
      y += tilemap->tile_size;
    }

  HGE_view_clipv(&view);

  HGE_glPopMatrix();
}

void
HGE_tilemap_draw(Uint32 n, HGE_tilemap_t** tilemap)
{
  while(n--)
    {
      HGE_tilemap_draw1(*tilemap);
      tilemap++;
    }
}
