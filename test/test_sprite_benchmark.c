#include <stdio.h>
#include "HGE.h"

#define NUM_SPRITES 100
#define MAX_SPEED   1

struct veloc_t
{
  GLfloat vx;
  GLfloat vy;
};

int xres,yres;

void
move_sprites(int n, HGE_sprite_t** s)
{
  struct veloc_t* veloc;

  while(n--)
    {
      veloc = (*s)->data;
      (*s)->x += veloc->vx;
      if(((*s)->x < 0) || ((*s)->x >= ((float)xres - (*s)->image->w)))
	{
	  veloc->vx = -veloc->vx;
	  (*s)->x += veloc->vx;
	}

      (*s)->y += veloc->vy;
      if(((*s)->y < 0) || ((*s)->y >= ((float)yres - (*s)->image->h)))
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
  int n;
  HGE_timer_t timer;
  HGE_image_t* image;
  HGE_sprite_t* sprites[NUM_SPRITES];
  struct veloc_t veloc[NUM_SPRITES];

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  HGE_config_set_texture_format(GL_COMPRESSED_RGBA);
  HGE_enable_alpha_channel();
  image = HGE_image_alloc_from_file("icon.png", NULL);

  for(n = 0; n < NUM_SPRITES; n++)
    {
      sprites[n]    = HGE_sprite_alloc();
      sprites[n]->x = HGE_random_float(0.0, (float)xres-image->w);
      sprites[n]->y = HGE_random_float(0.0, (float)yres-image->h);
      sprites[n]->image = image;
      sprites[n]->data  = &veloc[n];
      veloc[n].vx = veloc[n].vy = 0;
      while(veloc[n].vx == 0.0 && veloc[n].vy == 0.0)
	{
	  veloc[n].vx = HGE_random_float(-1.0f, 1.0f);
	  veloc[n].vy = HGE_random_float(-1.0f, 1.0f);
	}
    }

  HGE_timer_set(&timer, 30000);
  n = 0;
  while(!HGE_timer_done(&timer))
    {
      HGE_clearbuffer();
      
      HGE_sprite_drawpv(sprites, NUM_SPRITES);
      move_sprites(NUM_SPRITES, sprites);

      SDL_GL_SwapBuffers();
      n++;
    }

  printf("%d fps\n", n/30);

  for(n = 0; n < NUM_SPRITES; n++)
    HGE_sprite_free(sprites[n]);
  HGE_image_free(image);

  HGE_quit();

  return 0;
}
