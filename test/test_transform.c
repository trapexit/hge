#include <stdio.h>
#include "HGE.h"

#include "lilbent_gif.c"

int
main(int argc, char** argv)
{
  int x,y,done;
  HGE_framerate_t framerate;
  HGE_image_t  image;
  HGE_transform_t t;
  SDL_Surface* surface;
  SDL_Event event;
  int xres,yres;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  surface = IMG_Load_RW(SDL_RWFromConstMem(lilbent_gif, lilbent_gif_len), SDL_TRUE);
  HGE_image_make_from_surface(&image, surface, NULL);

  HGE_transform_init(&t);

  done = HGE_FALSE;
  HGE_framerate_set(&framerate, 60);
  float tx,ty;
  while(!done)
    {
      HGE_clearbuffer();

      while(SDL_PollEvent(&event))
	{
	  switch(event.type)
	    {
	    case SDL_KEYDOWN:
	      switch(event.key.keysym.sym)
		{
		case SDLK_UP:
		case SDLK_RIGHT:
		  t.scale += 0.01;
		  break;
		case SDLK_DOWN:
		case SDLK_LEFT:
		  t.scale -= 0.01;
		  break;
		default:
		  done = HGE_TRUE;
		  break;
		}
	      break;
	      
	    case SDL_MOUSEMOTION:
	      tx = (float)event.motion.x;
	      ty = (float)event.motion.y;
	      t.rotation = (float)event.motion.x;
	      break;

	    default:
	      break;
	    }
	}

      for(y = 0; y < yres; y+=image.h)
	{
	  for(x = 0; x < xres; x+=image.w)
	    {
	      HGE_image_draw_transformed(&image, x, y, &t);
	      t.flags ^= TRANSFORM_H_FLIP;
	    }
	  t.flags ^= TRANSFORM_V_FLIP;
	}
      

      SDL_GL_SwapBuffers();
      HGE_framerate_delay(&framerate);
    }

  SDL_Quit();
  
  return 0;
}
