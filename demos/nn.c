#include "HGE.h"

const char* bg_images[] =
  {
    "bg1.png",
    "e1.png",
    "i1.png",
    "e5.png",
    "e6.png",
    "i5.png",
    "i6.png"
  };

typedef struct NN_player_t NN_player_t;
struct NN_player_t
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
  HGE_image_t* js1_img[2];
  HGE_image_t* js2_img[2];
};

void
NN_background_init(NN_background_t* bg)
{
  int i;

  bg->bg  = NULL;
  bg->p1e = NULL;
  bg->p2e = NULL;
  bg->js1 = NULL;
  bg->js2 = NULL;

  for(i = 0; i < 2; i++)
    bg->js1_img[i] = bg->js2_img[i] = NULL;
}

void
NN_background_clean(NN_background_t* bg)
{
  int i;
  HGE_imap_free(bg->bg);
  HGE_imap_free(bg->p1e);
  HGE_imap_free(bg->p2e);
  HGE_sprite_free(bg->js1);
  HGE_sprite_free(bg->js2);
  for(i = 0; i < 2; i++)
    {
      HGE_image_free(bg->js1_img[i]);
      HGE_image_free(bg->js2_img[i]);
    }
}

int
NN_background_load(NN_background_t* bg)
{
  bg->bg = HGE_imap_alloc_from_file(bg_images[0]);
  bg->p1e = HGE_imap_alloc_from_file(bg_images[1]);
  bg->p2e = HGE_imap_alloc_from_file(bg_images[2]);
  bg->js1_img[0] = HGE_image_alloc_from_file(bg_images[3], NULL);
  bg->js1_img[1] = HGE_image_alloc_from_file(bg_images[4], NULL);
  bg->js1_img[0] = HGE_image_alloc_from_file(bg_images[5], NULL);
  bg->js1_img[1] = HGE_image_alloc_from_file(bg_images[6], NULL);

  return HGE_SUCCEEDED;
}

void
frame_func(HGE_loop_t* loop)
{
  SDL_Event event;
  NN_background_t* bg = HGE_loop_get_data(loop);;

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

  HGE_imap_draw(bg->bg, 0, 0);

  HGE_swapbuffers();
  SDL_Delay(10);
}

void
NN_main_battle(void)
{
  HGE_loop_t loop;
  NN_background_t* bg;

  HGE_enable_alpha_channel();
  HGE_color_default_blend();
  HGE_color_reset4();
  
  bg = HGE_alloc(NN_background_t);
  NN_background_init(bg);
  if(NN_background_load(bg) == HGE_SUCCEEDED)
    {
      HGE_loop_init(&loop);
      HGE_loop_set_framefunc(&loop, frame_func);
      HGE_loop_set_data(&loop, bg);
      HGE_loop_run(&loop);
      NN_background_clean(bg);
    }

  HGE_free(bg);
}

int
main(int argc, char** argv)
{
  HGE_cmdline_parse(argc, argv);
  
  HGE_config_set_resolution(320*2, 200*2);
  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_view_scale(0, 320, 0, 200);

  NN_main_battle();

  HGE_quit();

  return 0;
}
