#include <stdio.h>
#include "HGE.h"

int
main(int argc, char** argv)
{
  int x=0,y=0;
  int size;
  int comps[4] = {0};
  int done;
  SDL_Event event;
  HGE_framerate_t framerate;
  HGE_image_t* image;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_set_texture_format(GL_RGBA);
  HGE_enable_alpha_channel();
  HGE_color_default_blend();

  SDL_Rect r = {0,0,16,16};
  image = HGE_image_alloc_from_file(argv[1], &r);
  
  HGE_glBindTexture(GL_TEXTURE_2D, image->texture);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_ARB, &size);
  printf("%d\n", size);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB, &size);
  printf("%d\n", size);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &size);
  printf("%d\n", size);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &size);
  printf("%d\n", size);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, &size);
  printf("%d\n", size);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_GREEN_SIZE, &size);
  printf("%d\n", size);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BLUE_SIZE, &size);
  printf("%d\n", size);
  HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, &size);
  printf("%d\n", size);

  HGE_glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &size);
  printf("%d\n", size);

  HGE_glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, comps);
  printf("%x %x %x\n", comps[0], comps[1], comps[2]);
  
  done = HGE_FALSE;
  HGE_framerate_set(&framerate, 60);
  while(!done)
    {
      HGE_clearbuffer();
      
      while(SDL_PollEvent(&event))
	{
	  switch(event.type)
	    {
	    case SDL_MOUSEMOTION:
	      x = event.motion.x;
	      y = event.motion.y;
	      break;

	    case SDL_KEYDOWN:
	    case SDL_MOUSEBUTTONDOWN:
	    case SDL_QUIT:
	      done = HGE_TRUE;
	      break;

	    default:
	      break;
	    }
	}

      HGE_image_draw(image, x, y);

      SDL_GL_SwapBuffers();
      HGE_framerate_delay(&framerate);
    }

  HGE_image_free(image);

  HGE_quit();

  return 0;
}

