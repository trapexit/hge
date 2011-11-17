#include <stdio.h>
#include "HGE.h"

int
main(int argc, char** argv)
{
  int x=0,y=0;
  int done;
  SDL_Event event;
  HGE_framerate_t framerate;
  HGE_imap_t imap;
  SDL_Surface* surface;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

  surface = IMG_Load(argv[1]);
  HGE_imap_init(&imap);
  HGE_imap_make_from_surface(&imap, surface);
  
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
	      switch(event.key.keysym.sym)
		{
		case SDLK_UP:
		  y--;
		  break;
		case SDLK_DOWN:
		  y++;
		  break;
		case SDLK_RIGHT:
		  x++;
		  break;
		case SDLK_LEFT:
		  x--;
		  break;
		default:
		  done = HGE_TRUE;
		  break;
		}
	      break;

	    case SDL_MOUSEBUTTONDOWN:
	    case SDL_QUIT:
	      done = HGE_TRUE;
	      break;

	    default:
	      break;
	    }
	}

/*       angle += 0.1; */
/*       HGE_draw_imap_rotate(&imap, x, y, angle); */
      
      HGE_imap_draw(&imap, x, y);

      HGE_swapbuffers();
      HGE_framerate_delay(&framerate);
    }

  HGE_imap_clean(&imap);
  HGE_quit();

  return 0;
}
