#include "HGE.h"
#include "char_select.h"

static char* NN_cs_screen_name[] =
  {
    "ZIPPY",
    "GUTRIP",
    "HALFWIT",
    "BOGEY",
    "KLUBBOR",
    "KEWPUD"
  };

static char* NN_cs_screen_file[] =
  {
    "cs_screen_zippy.png",
    "cs_screen_gutrip.png",
    "cs_screen_halfwit.png",
    "cs_screen_bogey.png",
    "cs_screen_klubbor.png",
    "cs_screen_kewpud.png",
    "cs_screen_p1_arrow.png",
    "cs_screen_p2_arrow.png",
    "cs_screen_background.png",
    "cs_screen_font.png"
  };
    

HGE_typeinfo_t NN_cs_screen[] = {{sizeof(NN_cs_screen_t),
				  NN_cs_screen_init,
				  NN_cs_screen_clean,
				  NULL,
				  NULL}};

/* private */
void
NN_cs_screen_framefunc(HGE_loop_t* loop)
{
  char* name1;
  HGE_image_t* char1;
  NN_cs_screen_t* s = HGE_loop_get_data(loop);
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
	    case SDLK_LEFT:
	      if(s->p1 > 0)
		{
		  s->p1--;
		  s->arrow[0]->x = 85 + s->p1 * 24;
		}
	      break;
	    case SDLK_RIGHT:
	      if(s->p1 < 5)
		{
		  s->p1++;
		  s->arrow[0]->x = 85 + s->p1 * 24;
		}
	      break;
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

  HGE_image_draw(s->bg, 0, 0);

  char1 = s->charbust[s->p1];
  name1 = NN_cs_screen_name[s->p1];
  
  HGE_image_draw(char1, 0, (s->bg->h - char1->h));
  HGE_sprite_draw(s->arrow[0]);
/*   HGE_font_draw(s->font, */
/* 		(char1->w - HGE_font_str_length(s->font, name1))/2, */
/* 		s->bg->h - char1->h - HGE_font_str_height(s->font, name1), */
/* 		name1); */
  HGE_font_draw(s->font, 1, 1, name1);


  HGE_color_reset3();
  
  HGE_swapbuffers();
  SDL_Delay(10);
}

/* public */
HGE_error_t
NN_cs_screen_init(void* ptr)
{
  int i;
  NN_cs_screen_t* s = ptr;

  s->loop = HGE_loop_alloc();
  if(s->loop == NULL)
    goto error;
  HGE_loop_set_framefunc(s->loop, NN_cs_screen_framefunc);
  HGE_loop_set_data(s->loop, s);

  s->bg = HGE_image_alloc_from_file(NN_cs_screen_file[BACKGROUND], NULL);
  if(s->bg == NULL)
    goto error;

  s->font = HGE_font_alloc_from_hge(NN_cs_screen_file[FONT], ' ');
  if(s->font == NULL)
    goto error;

  s->arrow[0] = HGE_sprite_alloc_from_file(NN_cs_screen_file[P1_ARROW], NULL);
  if(s->arrow[0] == NULL)
    goto error;

  s->arrow[1] = HGE_sprite_alloc_from_file(NN_cs_screen_file[P2_ARROW], NULL);
  if(s->arrow[1] == NULL)
    goto error;

  for(i = ZIPPY; i <= KEWPUD; i++)
    {
      s->charbust[i] = HGE_image_alloc_from_file(NN_cs_screen_file[i], NULL);
      if(s->charbust[i] == NULL)
	goto error;
    }

  s->arrow[0]->x = 85;
  s->arrow[0]->y = 30;
  s->p1 = 0;
  s->p2 = 5;

  return HGE_SUCCESS;

 error:
  NN_cs_screen_clean(s);
  return HGE_FAILURE;
}

void
NN_cs_screen_clean(void* ptr)
{
  int i;
  NN_cs_screen_t* s = ptr;

  if(ptr != NULL)
    {
      HGE_loop_free(s->loop);
      HGE_image_free(s->bg);
      HGE_font_free(s->font);
      HGE_sprite_free(s->arrow[0]);
      HGE_sprite_free(s->arrow[1]);
      for(i = ZIPPY; i <= KEWPUD ; i++)
	HGE_image_free(s->charbust[i]);
    }
}

void
NN_cs_screen_run(NN_cs_screen_t* s)
{
  HGE_enable_alpha_channel();
  HGE_color_default_blend();
  HGE_loop_run(s->loop);
}

int
NN_cs_screen_get_p1(const NN_cs_screen_t* s)
{
  return s->p1;
}

int
NN_cs_screen_get_p2(const NN_cs_screen_t* s)
{
  return s->p2;
}

const char*
NN_cs_screen_get_p1_name(const NN_cs_screen_t* s)
{
  return NN_cs_screen_name[NN_cs_screen_get_p1(s)];
}

const char*
NN_cs_screen_get_p2_name(const NN_cs_screen_t* s)
{
  return NN_cs_screen_name[NN_cs_screen_get_p2(s)];
}
  
int
main(int argc, char** argv)
{
  NN_cs_screen_t* cs_screen;

  HGE_cmdline_parse(argc, argv);
  
  HGE_config_set_resolution(320*2, 200*2);
  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_view_scale(0, 319, 0, 199);

  cs_screen = HGE_new(NN_cs_screen);

  NN_cs_screen_run(cs_screen);

  printf("Player 1: %s\n", 
	 NN_cs_screen_name[NN_cs_screen_get_p1(cs_screen)]);

  HGE_delete(cs_screen);

  HGE_quit();
  return 0;
}
