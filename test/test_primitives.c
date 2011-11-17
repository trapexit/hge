#include <stdio.h>
#include "HGE.h"

struct line_t
{
  float x0,y0;
  float x1,y1;
};

int
main(int argc, char** argv)
{
  int xres, yres, done, n;
  Uint32 start, stop;
  struct line_t lines[4];
  SDL_Event event;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  lines[0].x0 = 0;
  lines[0].y0 = 0;
  lines[1].x0 = xres;
  lines[1].y0 = 0;
  lines[2].x0 = xres;
  lines[2].y0 = yres;
  lines[3].x0 = 0;
  lines[3].y0 = yres;

  HGE_color_set3(0,0,255);

  n = done = 0;
  start = SDL_GetTicks();
  while(!done)
    {
      while(SDL_PollEvent(&event))
	{
	  switch(event.type)
	    {
	    case SDL_MOUSEBUTTONDOWN:
	    case SDL_QUIT:
	    case SDL_KEYDOWN:
	      done = HGE_TRUE;
	      break;
	    case SDL_MOUSEMOTION:
	      lines[0].x1 = lines[1].x1 = lines[2].x1 = lines[3].x1 = (float)event.motion.x;
	      lines[0].y1 = lines[1].y1 = lines[2].y1 = lines[3].y1 = (float)event.motion.y;
	      break;
	    default:
	      break;
	    }
	}

      HGE_clearbuffer();

      HGE_draw_linefv((HGE_spfloat_t*)lines, 4);

      HGE_swapbuffers();

      n++;
    }
  stop = SDL_GetTicks();

  printf("%d Frames\n", n);
  printf("%f Seconds\n", ((float)(stop-start) / 1000.0));
  printf("%f FPS\n", ((float)n / ((float)(stop-start) / 1000.0)));


  HGE_quit();

  return 0;
}
