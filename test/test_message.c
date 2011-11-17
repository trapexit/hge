#include <stdio.h>
#include "HGE.h"

int
main(int argc, char** argv)
{
  int done;
  int xres,yres;
  SDL_Event event;
  HGE_framerate_t framerate;
  HGE_sprite_t* spr;
  HGE_argv_t* args;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  HGE_enable_alpha_channel();
  spr = HGE_sprite_alloc_from_file("icon.png", NULL);

  HGE_postoffice_set_fh(spr, HGE_sprite);
  args = HGE_argv_alloc();
  HGE_argv_set_float(args, 0, 50.0);
  HGE_postoffice_send(SDL_GetTicks() + 1000, 0, spr, HGE_SPR_SET_X, args); 

  args = HGE_argv_alloc();
  HGE_argv_set_float(args, 0, 10.0);
  HGE_postoffice_send(SDL_GetTicks() + 2000, 0, spr, HGE_SPR_INC_X, args);

  args = HGE_argv_alloc();
  HGE_argv_set_float(args, 0, 1);
  HGE_argv_set_float(args, 1, (float)xres);
  HGE_postoffice_send(SDL_GetTicks() + 3000, 0, spr, HGE_SPR_CINC_X, args);

  args = HGE_argv_set_from_string(NULL, "fff", 200.0, 200.0, 5.0);
  HGE_postoffice_send(SDL_GetTicks() + 6000, 0, spr, HGE_SPR_GOTO_XY, args);

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

      HGE_postoffice_process();
      HGE_sprite_draw(spr);

      HGE_swapbuffers();
      HGE_framerate_delay(&framerate);
    }

  HGE_image_free(spr->image);
  HGE_sprite_free(spr);

  HGE_quit();

  return 0;
}
