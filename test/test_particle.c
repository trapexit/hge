#include "HGE.h"

HGE_font_t* font;
HGE_emitter_t* e;

void
frame_func(HGE_loop_t* loop)
{
  static float px,py;
  SDL_Event event;

  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_KEYDOWN:
	  switch(event.key.keysym.sym)
	    {
	    case SDLK_a:
	      HGE_glEnable(GL_POINT_SMOOTH);
	      HGE_glEnable(GL_LINE_SMOOTH);
	      break;
	    case SDLK_s:
	      HGE_glDisable(GL_POINT_SMOOTH);
	      HGE_glDisable(GL_LINE_SMOOTH);
	      break;
	    case SDLK_ESCAPE:
	      loop->done = HGE_TRUE;
	      break;
	    case SDLK_1:
	    case SDLK_2:
	    case SDLK_3:
	    case SDLK_4:
	    case SDLK_5:
	    case SDLK_6:
	    case SDLK_7:
	    case SDLK_8:
	    case SDLK_9:
	      e->size = (float)(event.key.keysym.sym - SDLK_0);
	      break;
	    case SDLK_UP:
	      e->pps += 100.0f;
	      break;
	    case SDLK_DOWN:
	      e->pps -= 100.0f;
	      break;
	    case SDLK_LEFT:
	      e->direction -= 10.0f;
	      break;
	    case SDLK_RIGHT:
	      e->direction += 10.0f;
	      break;
	    case SDLK_l:
	      HGE_emitter_set_type(e, HGE_EMITTER_LINES);
	      break;
	    case SDLK_p:
	      HGE_emitter_set_type(e, HGE_EMITTER_POINTS);
	      break;
	    case SDLK_o:
	      HGE_emitter_set_type(e, HGE_EMITTER_POINT_SPRITES);
	      break;
	    case SDLK_k:
	      HGE_emitter_set_type(e, HGE_EMITTER_QUADS);
	      break;
	    case SDLK_SPACE:
	      e->paused = !e->paused;
	      break;
	    default:
	      break;
	    }
	  break;
	case SDL_MOUSEMOTION:
	  px = (float)event.motion.x;
	  py = (float)event.motion.y;
	  break;
	case SDL_QUIT:
	  loop->done = HGE_TRUE;
	  break;
	default:
	  break;
	}
    }

  HGE_clearbuffer();

  HGE_color_reset3();
  HGE_font_printf(font, 0, 0, "FPS: %d\nDelta T: %f", loop->fps, loop->dt);
  HGE_emitter_update(e, loop->dt);
  HGE_emitter_draw(e, px, py);

  HGE_swapbuffers();
}

int
main(int argc, char** argv)
{
  float t;
  HGE_image_t img;
  HGE_loop_t loop;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();
  
  font = HGE_font_alloc();
  HGE_font_load(font, "font_10x20.png", ' ', '~', 10, 20);

  e = HGE_emitter_alloc();
  HGE_emitter_set_size(e, 10000);
  e->pps     = 1000.0f;
  e->age_min = 0.9f;
  e->age_max = 1.1f;
  e->size    = 1;
  e->direction = 270.0f;
  e->spread = 5.0f;
  e->x_min  = -10.0f;
  e->x_max  = 10.0f;
  e->vx_min = 100.0f;
  e->vx_max = 100.0f;
  e->vy_min = 100.0f;
  e->vy_max = 100.0f;
  e->ax_min = 0.0f;
  e->ax_max = 0.0f;
  e->ay_min = 0.0f;
  e->ay_max = 0.0f;
  e->r_s = 255;
  e->g_s = 0;
  e->b_s = 0;
  e->a_s = 255;
  e->r_e = 255;
  e->g_e = 255;
  e->b_e = 10;
  e->a_e = 0;
  
  HGE_glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, &t);
  printf("SIZE_RANGE %f\n", t);
  HGE_glGetFloatv(GL_SMOOTH_POINT_SIZE_GRANULARITY, &t);
  printf("SIZE_GRANULARIY %f\n", t);
  HGE_glGetFloatv(GL_POINT_SIZE_MIN, &t);
  printf("SIZE_MIN %f\n", t);
  HGE_glGetFloatv(GL_POINT_SIZE_MAX, &t);
  printf("SIZE_MAX %f\n", t);

  HGE_enable_alpha_channel();
  HGE_color_default_blend();

  if(argv[1])
    HGE_image_make_from_file(&img, argv[1], NULL);
  
  e->extra = (void*)img.texture;

  HGE_loop_init(&loop);
  HGE_loop_set_framefunc(&loop, frame_func);

  HGE_loop_run(&loop);

  HGE_image_clean(&img);
  HGE_font_free(font);
  HGE_emitter_free(e);

  HGE_quit();
  return 0;
}
