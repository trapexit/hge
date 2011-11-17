#include <stdio.h>
#include "HGE.h"

#define NUM_SPRITES 1000
#define MAX_SPEED   2

struct veloc_t
{
  GLfloat vx;
  GLfloat vy;
};

int xres,yres;

void
move_sprites(HGE_sprite_t** s, int n)
{
  struct veloc_t* veloc;

  while(n--)
    {
      veloc = (*s)->data;
      (*s)->x += veloc->vx;
      if(((*s)->x < 0) || ((*s)->x >= ((GLfloat)xres - (*s)->image->w)))
	{
	  veloc->vx = -veloc->vx;
	  (*s)->x += veloc->vx;
	}

      (*s)->y += veloc->vy;
      if(((*s)->y < 0) || ((*s)->y >= ((GLfloat)yres - (*s)->image->h)))
	{
	  veloc->vy = -veloc->vy;
	  (*s)->y += veloc->vy;
	}

      s++;
    }
}

int
main(int argc, char** argv)
{
  int done, n;
  SDL_Event event;
  HGE_framerate_t framerate;
  HGE_image_t* image;
  HGE_sprite_t* sprites[NUM_SPRITES];
  struct veloc_t veloc[NUM_SPRITES];

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  HGE_enable_alpha_channel();
  HGE_color_default_blend();

  image = HGE_image_alloc_from_file("icon.png", NULL);

  for(n = 0; n < NUM_SPRITES; n++)
    {
      sprites[n] = HGE_sprite_alloc();
      sprites[n]->x = HGE_random_float(0.0, (GLfloat)xres - image->w);
      sprites[n]->y = HGE_random_float(0.0, (GLfloat)yres - image->h);
      sprites[n]->image = image;
      sprites[n]->data  = &veloc[n];
      veloc[n].vx = veloc[n].vy = 0.0;
      while(veloc[n].vx == 0.0 && veloc[n].vy == 0.0)
	{
	  veloc[n].vx = HGE_random_float(-1.0f, 1.0f);
	  veloc[n].vy = HGE_random_float(-1.0f, 1.0f);
	}
    }

  done = HGE_FALSE;
  HGE_framerate_set(&framerate, 60);
  while(!done)
    {
      HGE_clearbuffer();
      
      while(SDL_PollEvent(&event))
	{
	  switch(event.type)
	    {
	    case SDL_KEYDOWN:
	    case SDL_MOUSEBUTTONDOWN:
	    case SDL_QUIT:
	      done = HGE_TRUE;
	      break;

	    default:
	      break;
	    }
	}

      HGE_sprite_drawpv(sprites, NUM_SPRITES);
      move_sprites(sprites, NUM_SPRITES);

      HGE_swapbuffers();

      HGE_framerate_delay(&framerate);      
    }

  for(n = 0; n < NUM_SPRITES; n++)
    HGE_sprite_free(sprites[n]);
  HGE_image_free(image);

  HGE_quit();

  return 0;
}
