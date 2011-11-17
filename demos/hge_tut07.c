#include <stdio.h>
#include <stdlib.h>
#include "HGE.h"

#define NUM_OBJECTS 100

typedef struct
{
  GLfloat dx,dy;
  GLfloat dscale;
  GLfloat drot;
  Uint8   r,g,b;
}Bunny;

int xres,yres;
HGE_font_t* font;
HGE_image_t*  image[2];
HGE_sprite_t* spr[NUM_OBJECTS];
Bunny bunny[NUM_OBJECTS];

void
move_sprites(Bunny* bunny, HGE_sprite_t** spr, float dt)
{
  int i;

  for(i = 0; i < NUM_OBJECTS; i++)
    {
      spr[i]->x += bunny[i].dx * dt;
      if(spr[i]->x > xres || spr[i]->x < 0)
	bunny[i].dx = -bunny[i].dx;
      spr[i]->y += bunny[i].dy * dt;
      if(spr[i]->y > yres || spr[i]->y < 0)
	bunny[i].dy = -bunny[i].dy;
      spr[i]->transform.scale += bunny[i].dscale * dt;
      if(spr[i]->transform.scale > 2.0 || spr[i]->transform.scale < 0.5)
	bunny[i].dscale = -bunny[i].dscale;

      spr[i]->transform.rotation += bunny[i].drot * dt;
    }
}

void
frame_func(HGE_loop_t* loop)
{
  int i;
  SDL_Event event;

  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	case SDL_QUIT:
	case SDL_KEYDOWN:
	  loop->done = HGE_TRUE;
	  break;
	case SDL_MOUSEMOTION:
	  break;
	default:
	  break;
	}
    }
    
  HGE_color_reset3();
  HGE_image_draw(image[1], 0, 0);

  for(i = 0; i < NUM_OBJECTS; i++)
    {
      HGE_color_set3(bunny[i].r, bunny[i].g, bunny[i].b);
      HGE_sprite_draw(spr[i]);
    }

  HGE_color_reset4();
  HGE_font_printf(font, 0, 0, "FPS: %d", HGE_loop_get_fps(loop));

  HGE_swapbuffers();

  move_sprites(bunny, spr, HGE_loop_get_dt(loop));
}

int
main(int argc, char** argv)
{
  int i;
  HGE_loop_t loop;
    
  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();
  printf("init done\n");
  
  HGE_config_get_resolution(&xres, &yres);
  HGE_enable_alpha_channel();
  HGE_config_set_texture_format(GL_COMPRESSED_RGBA);

  image[0] = HGE_image_alloc_from_file("zazaka.png", NULL);
  HGE_texture_set_filter(image[0]->texture, GL_LINEAR);

  image[1] = HGE_image_alloc_from_file("bg2.png", NULL);
  HGE_image_make_tiled(image[1], xres, yres);

  for(i = 0; i < NUM_OBJECTS; i++)
    {
      spr[i] = HGE_sprite_alloc();
      spr[i]->x                           = HGE_random_float(0, (GLfloat)xres-image[0]->w);
      spr[i]->y                           = HGE_random_float(0, (GLfloat)yres-image[0]->h);
      spr[i]->image                       = image[0];
      bunny[i].dx                         = HGE_random_float(-200, 200);
      bunny[i].dy                         = HGE_random_float(-200, 200);
      spr[i]->transform.scale             = HGE_random_float(.5f, 2.0f);
      bunny[i].dscale                     = HGE_random_float(-.5f, 1.0f);
      spr[i]->transform.rotation          = HGE_random_float(0, 360);
      bunny[i].drot                       = HGE_random_float(-60.0f, 60.0f);
      bunny[i].r                          = HGE_random_int(0, 255);
      bunny[i].g                          = HGE_random_int(0, 255);
      bunny[i].b                          = HGE_random_int(0, 255);
    }

  font = HGE_font_alloc();
  HGE_font_load(font, "font_10x20.png", ' ', '~', 10, 20);
  printf("font loaded\n");

  HGE_color_default_blend();

  HGE_loop_init(&loop);
  HGE_loop_set_framefunc(&loop, frame_func);

  HGE_loop_run(&loop);

  for(i = 0; i < NUM_OBJECTS; i++)
    HGE_sprite_free(spr[i]);
  HGE_image_free(image[0]);
  HGE_image_free(image[1]);
  HGE_font_free(font);

  HGE_quit();
  return 0;
}

