#include <stdio.h>
#include "HGE.h"

int
main(int argc, char** argv)
{						
  int done;
  SDL_Rect rect;
  SDL_Event ev;
  HGE_framerate_t framerate;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  rect.x = rect.y = 0;
  rect.w = rect.h = 64;

  HGE_trigger_init_system();
  HGE_trigger_add(HGE_MOUSE_FOCUS, NULL, 1, &rect, 0);

  done = 0;
  HGE_framerate_set(&framerate, 60);
  while(!done)
    {
      while(SDL_PollEvent(&ev))
	{
	  switch(ev.type)
	    {
	    case SDL_QUIT:
	      done = 1;
	      break;
	    }
	}
      HGE_framerate_delay(&framerate);
    }

  HGE_quit();

  return 0;
}

