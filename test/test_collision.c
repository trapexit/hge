#include <stdio.h>
#include "HGE.h"


int
main(int argc, char** argv)
{
  HGE_framerate_t framerate;
  int i;
  SDL_Rect r0, r1;

  HGE_config_set_fullscreen(HGE_FALSE);
  HGE_config_set_audio(HGE_FALSE);
  HGE_config_set_resolution(640, 480);

  HGE_initialize_video();
  HGE_initialize_timer();

  r0.x = 50;
  r0.y = 50;
  r0.w = 100;
  r0.h = 100;

  r1.x = 300;
  r1.y = 50;
  r1.w = 50;
  r1.h = 50;

  i = 320;
  HGE_framerate_set(&framerate, 30);
  while(i--)
    {
      HGE_clearbuffer();

      if(!HGE_misc_rect_collision(&r0, &r1))
	r1.x--;

      HGE_color_set3(255, 0, 0);
      HGE_draw_sdlrect(&r0);

      HGE_color_set3(0, 255, 0);
      HGE_draw_sdlrect(&r1);

      fprintf(stderr, "%d %d %d\n", HGE_misc_rect_collision(&r0, &r1), r0.x, r1.x);

      

      SDL_GL_SwapBuffers();
      HGE_framerate_delay(&framerate);
    }

  SDL_Quit();

  return 0;
}
