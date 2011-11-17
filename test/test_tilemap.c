#include <stdio.h>
#include "HGE.h"

HGE_tilemap_t* create_map(void);

int
main(int argc, char** argv)
{
  int i;
  HGE_framerate_t framerate;
  HGE_tilemap_t* map;

  HGE_config_set_fullscreen(HGE_FALSE);
  HGE_config_set_audio(HGE_FALSE);
  HGE_config_set_resolution(640, 480);

  HGE_initialize_video();
  HGE_initialize_timer();
  
  map = create_map();
  
  i = 300;
  HGE_framerate_set(&framerate, 30);
  while(i--)
    {
      HGE_tilemap_draw1(map);
      HGE_tilemap_inc_coord(map, 1, 0);

      SDL_GL_SwapBuffers();
      HGE_framerate_delay(&framerate);
    }
  
  HGE_quit();

  return 0;
}

HGE_tilemap_t* create_map(void)
{
  int i, j;
  HGE_image_t* image;
  HGE_tilemap_t* map;

  map = HGE_tilemap_alloc();
  HGE_tilemap_set_tile_size(map, 32);
  HGE_tilemap_set_map_size(map, 10, 10);
  HGE_tilemap_set_coord(map, 0, 0);
  HGE_tilemap_set_display_size(map, 5, 5);

  image = HGE_image_alloc_from_file("tile0.png", NULL);
  for(j = 0; j < 10; j+=2)
    {
      for(i = 0; i < 10; i++)
	{
	  HGE_tilemap_set_tile(map, i, j, image);
	}
    }

  image = HGE_image_alloc_from_file("tile1.png", NULL);
  for(j = 1; j < 10; j+=2)
    {
      for(i = 0; i < 10; i++)
	{
	  HGE_tilemap_set_tile(map, i, j, image);
	}
    }
  
  return map;
}
