#include "HGE.h"

int
main(int argc, char** argv)
{
  SDL_Surface* s;

  if(argc > 1)
    {
      s = IMG_Load(argv[1]);
      if(s)
	{
	  printf("GLtype:        %X\n", HGE_surface_guess_gl_type(s));
	  printf("GLformat:      %X\n", HGE_surface_guess_gl_format(s));
	  printf("BitsPerPixel:  %d\n"
		 "BytesPerPixel: %d\n"
		 "Rmask:         %.8X\n"
		 "Gmask:         %.8X\n"
		 "Bmask:         %.8X\n"
		 "Amask:         %.8X\n"
		 "RmaskSwap:     %.8X\n"
		 "GmaskSwap:     %.8X\n"
		 "BmaskSwap:     %.8X\n"
		 "AmaskSwap:     %.8X\n"
		 "Rshift:        %d\n"	     
		 "Gshift:        %d\n"	     
		 "Bshift:        %d\n"	     
		 "Ashift:        %d\n"	     
		 "Rloss:         %d\n"	     
		 "Gloss:         %d\n"	     
		 "Bloss:         %d\n"	     
		 "Aloss:         %d\n",
		 s->format->BitsPerPixel,
		 s->format->BytesPerPixel,
		 s->format->Rmask,
		 s->format->Gmask,
		 s->format->Bmask,
		 s->format->Amask,
		 SDL_Swap32(s->format->Rmask),
		 SDL_Swap32(s->format->Gmask),
		 SDL_Swap32(s->format->Bmask),
		 SDL_Swap32(s->format->Amask),
		 s->format->Rshift,
		 s->format->Gshift,
		 s->format->Bshift,
		 s->format->Ashift,
		 s->format->Rloss,
		 s->format->Gloss,
		 s->format->Bloss,
		 s->format->Aloss
		 );
	}
    }

  return 0;
}
