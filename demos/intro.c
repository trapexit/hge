#include "HGE.h"

typedef struct NN_player_t NN_player_t;
struct
NN_player_t
{
  void (*think)(NN_player_t*,SDL_Event*);
  void (*draw)(NN_player_t*);
  HGE_image_t* imgs;
  float x,y;
  float vx,vy;
  float ax,ay;
  void* extra;
};

typedef struct NN_background_t NN_background_t;
struct NN_background_t
{
  HGE_imap_t* bg;
  HGE_imap_t* p1e;
  HGE_imap_t* p2e;
  HGE_sprite_t* js1;
  HGE_sprite_t* js2;
};

void
frame_func(HGE_loop_t* loop)
{
  SDL_Event event;

  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_QUIT:
	  loop->done = HGE_TRUE;
	  break;
	case SDL_MOUSEMOTION:
	  break;
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	    {
	    default:
	      loop->done = 1;
	      break;
	    }
	  break;
	default:
	  break;
	}
    }

  HGE_clearbuffer();

  HGE_swapbuffers();
  SDL_Delay(10);
}

int
main(int argc, char** argv)
{
  HGE_loop_t loop;
  HGE_cmdline_parse(argc, argv);
  
  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_loop_init(&loop);
  HGE_loop_set_framefunc(&loop, frame_func);
  HGE_loop_run(&loop);

  HGE_quit();
  return 0;
}
