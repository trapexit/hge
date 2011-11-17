#include <stdio.h>
#include "HGE.h"

HGE_font_t* font;

void
frame_func(HGE_loop_t* loop)
{
  SDL_Event event;
  int x,y;

  HGE_clearbuffer();

  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_QUIT:
	  loop->done = HGE_TRUE;
	  break;
	case SDL_KEYDOWN:
	  if(event.key.keysym.sym == SDLK_q)
	    loop->done = HGE_TRUE;
	  break;
	  
	default:
	  break;
	}
    }

  
  HGE_font_printf(font, 0, 0, "FPS: %d", loop->fps);
  SDL_GetMouseState(&x, &y);
  HGE_image_draw(loop->data, (float)x, (float)y);  

  HGE_swapbuffers();
}

int
main(int argc, char** argv)
{
  int xres,yres;
  HGE_image_t* image;
  HGE_loop_t loop;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  HGE_config_set_texture_format(GL_COMPRESSED_RGBA);

  HGE_enable_alpha_channel();
  HGE_color_default_blend();

  image = HGE_image_new_from_file("icon.png", NULL);

  font = HGE_new(HGE_font);
  HGE_font_load(font, "font_10x20.png", ' ', '~', 10, 20);

  HGE_loop_init(&loop);

  HGE_loop_set_framefunc(&loop, frame_func);
  HGE_loop_set_data(&loop, image);

  HGE_loop_run(&loop);

  HGE_unref(font);
  HGE_unref(image);

  HGE_quit();

  return 0;
}
