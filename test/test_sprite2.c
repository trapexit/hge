#include <stdio.h>
#include "HGE.h"

#define NUM_SPRITES 100
#define MAX_SPEED   2

struct veloc_t
{
  int vx;
  int vy;
};

int xres,yres;

void
move_sprites(HGE_sprite_t* spr, int n)
{
  struct veloc_t* veloc;

  while(n--)
    {
      veloc = spr->data;
      spr->x += veloc->vx;
      if((spr->x < 0) || (spr->x >= (xres - spr->image->w)))
	{
	  veloc->vx = -veloc->vx;
	  spr->x += veloc->vx;
	}

      spr->y += veloc->vy;
      if((spr->y < 0) || (spr->y >= (yres - spr->image->h)))
	{
	  veloc->vy = -veloc->vy;
	  spr->y += veloc->vy;
	}

      spr++;
    }
}

int
main(int argc, char** argv)
{
  int done, n;
  SDL_Event event;
  HGE_framerate_t framerate;
  HGE_image_t image;
  HGE_sprite_t sprites[NUM_SPRITES];
  struct veloc_t veloc[NUM_SPRITES];

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  HGE_enable_alpha_channel();
  HGE_image_make_from_file(&image, "icon.png", NULL);

  for(n = 0; n < NUM_SPRITES; n++)
    {
      HGE_sprite_init(&sprites[n]);
      HGE_sprite_set_xy(&sprites[n],
			HGE_random_int(0, xres-image.w), 
			HGE_random_int(0, yres-image.h));
      HGE_sprite_set_image(&sprites[n], &image);
      HGE_sprite_set_data(&sprites[n], &veloc[n]);
      veloc[n].vx = veloc[n].vy = 0;
      while(!veloc[n].vx && !veloc[n].vy)
	{
	  veloc[n].vx = (rand()%(MAX_SPEED*2+1))-MAX_SPEED;
	  veloc[n].vy = (rand()%(MAX_SPEED*2+1))-MAX_SPEED;
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

      
      HGE_sprite_drawv(sprites, NUM_SPRITES);
      move_sprites(sprites, NUM_SPRITES);

      SDL_GL_SwapBuffers();
      HGE_framerate_delay(&framerate);
    }

  HGE_quit();

  return 0;
}
