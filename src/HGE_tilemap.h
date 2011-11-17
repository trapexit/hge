#ifndef _HGE_TILE_MAP_H_
#define _HGE_TILE_MAP_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_image.h"
#include "HGE_message.h"

typedef struct HGE_tilemap_t HGE_tilemap_t;
struct HGE_tilemap_t
{
  float   x,y;
  int     map_x;
  int     map_y;
  int     tile_x;
  int     tile_y;
  int     x_off;
  int     y_off;
  float   pixel_draw_w;
  float   pixel_draw_h;
  Uint32  tiles_wide;
  Uint32  tiles_high;
  float   tile_size;
  Uint32  tile_size_mask;
  Uint32  tile_size_shift;
  HGE_image_t** map;
};

HGE_error_t HGE_tilemap_init(void* ptr);
void        HGE_tilemap_clean(void* ptr);
void*       HGE_tilemap_alloc(void);
void        HGE_tilemap_free(void* ptr);

void HGE_tilemap_set_tile_size(HGE_tilemap_t* map, Uint32 size);
void HGE_tilemap_set_map_size(HGE_tilemap_t* map, Uint32 w, Uint32 h);
void HGE_tilemap_get_map_size(HGE_tilemap_t* map, Uint32* w, Uint32* h);
void HGE_tilemap_set_tile(HGE_tilemap_t* map, Uint32 w, Uint32 h, HGE_image_t* image);
HGE_image_t* HGE_tilemap_get_tile(HGE_tilemap_t* map, Uint32 w, Uint32 h);
void HGE_tilemap_set_display_size(HGE_tilemap_t* map, Uint32 w, Uint32 h);
void HGE_tilemap_set_coord(HGE_tilemap_t* map, Uint32 x, Uint32 y);
void HGE_tilemap_inc_coord(HGE_tilemap_t* map, Sint32 x, Sint32 y);

void HGE_tilemap_draw1(HGE_tilemap_t* tilemap);
void HGE_tilemap_draw(Uint32 n, HGE_tilemap_t** tilemap);
#endif
