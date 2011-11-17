#include <stdio.h>
#include "HGE.h"

int
main(int argc, char** argv)
{
  int i,j;
  int w,h;
  HGE_image_t* img;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  img = HGE_image_alloc_from_file("icon.png", NULL);

  w = 4;
  h = 3;
  for(j = 0; j < 3; j++)
    for(i = 0; i < 4; i++)
      {
	printf("%d:%d offset = %d\n", 
	       i,j,
	       (((j/2) * (w*2)) + ((j&1) + ((j/2)&1) + (i*2))));
      }


  SDL_GL_SwapBuffers();

  HGE_image_free(img);

  HGE_quit();

  return 0;
}

