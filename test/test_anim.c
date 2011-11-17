#include <stdio.h>
#include "HGE.h"

void load_animation(HGE_anim_t* anim);

void
frame_func(HGE_loop_t* loop)
{
  SDL_Event event;
  
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
	
  HGE_anim_update(loop->data, loop->dt);
  HGE_anim_draw(loop->data, 0.0f, 0.0f);

  HGE_swapbuffers();
}

int
main(int argc, char** argv)
{
  HGE_loop_t loop;
  HGE_anim_t* anim;

  HGE_initialize_video();
  HGE_initialize_timer();

  anim = HGE_new(HGE_anim);

  HGE_loop_init(&loop);
  HGE_loop_set_data(&loop, anim);
  HGE_loop_set_framefunc(&loop, frame_func);
  
  load_animation(anim);
  anim->speed = 2.0f;
  HGE_anim_set_attr(anim, HGE_ANIM_REPEAT|HGE_ANIM_BOUNCE);

  HGE_loop_run(&loop);

  HGE_unref(anim);

  HGE_quit();

  return 0;
}

void
load_animation(HGE_anim_t* anim)
{
  HGE_image_t* img;

  img = HGE_image_new_from_file("tile0.png", NULL);
  HGE_anim_add_frame(anim, img);

  img = HGE_image_new_from_file("tile1.png", NULL);
  HGE_anim_add_frame(anim, img);

  img = HGE_image_new_from_file("tile2.png", NULL);
  HGE_anim_add_frame(anim, img);
}
