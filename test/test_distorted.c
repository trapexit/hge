#include <stdio.h>
#include <math.h>
#include "HGE.h"

#include "texture.png.c"

int
main(int argc, char** argv)
{
  int done, frames, i,j;
  HGE_image_t* img;
  HGE_distorted_t* d;
  SDL_Surface* surface;
  SDL_Event event;
  float cur_time;
  GLfloat disx,disy;
  Uint32 start_time,stop_time;
  SDLKey last_key;
  HGE_transform_t t;
  GLfloat rotdelta = 0.0;
  SDL_RWops* rwop;

  HGE_config_set_resolution(800,600);

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  rwop = SDL_RWFromConstMem(texture_png, texture_png_len);
  surface = IMG_Load_RW(rwop, HGE_TRUE);
  img = HGE_image_alloc_from_surface(surface, NULL);
  SDL_FreeSurface(surface);

  d = HGE_distorted_alloc();
  HGE_distorted_set_image(d, img);
  HGE_distorted_set_mesh_size(d, 16, 16);

  HGE_transform_init(&t);

  HGE_texture_set_filter(img->texture, GL_LINEAR);

  last_key = 0;
  disx = disy = 0.0;
  frames = 0;
  done = HGE_FALSE;
  cur_time = HGE_time_getf();
  start_time = HGE_time_geti();
  while(!done)
    {
      while(SDL_PollEvent(&event))
	{
	  switch(event.type)
	    {
	    case SDL_MOUSEBUTTONDOWN:
	      break;
	    case SDL_MOUSEMOTION:
	      break;
	    case SDL_KEYDOWN:
	      switch(event.key.keysym.sym)
		{
		case SDLK_q:
		  done = HGE_TRUE;
		  break;
		case SDLK_UP:
		  t.scale += 0.05;
		  break;
		case SDLK_DOWN:
		  t.scale -= 0.05;
		  break;
		case SDLK_LEFT:
		  rotdelta += -10.0;
		  break;
		case SDLK_RIGHT:
		  rotdelta += 10.0;
		  break;
		case SDLK_r:
		  t.scale = 1.0;
		  rotdelta = 0.0;
		  t.rotation = 0.0;
		case SDLK_1:
		case SDLK_2:
		case SDLK_3:
		  HGE_distorted_reset_mesh(d);
		default:
		  last_key = event.key.keysym.sym;
		  break;
		}
	      break;
	    case SDL_QUIT:
	      done = HGE_TRUE;
	      break;
	    default:
	      break;
	    }
	}

      HGE_clearbuffer();

      switch(last_key)
	{
	case SDLK_1:
	  for(i = 1; i < d->grid_h-1; i++)
	    for(j = 1; j < d->grid_w-1; j++)
	      {
		HGE_distorted_set_displacement(d, 
					       j, i,
					       cosf(cur_time*10+(i+j)/2)*5,
					       sinf(cur_time*10+(i+j)/2)*5,
					       HGE_DISTORT_POINT_ABS);
	      }
      	  break;
	case SDLK_2:
	  for(i = 0; i < d->grid_h; i++)
	    for(j = 1; j < d->grid_w-1; j++)
	      {
		int col;
		HGE_distorted_set_displacement(d, 
					       j, i,
					       cosf(cur_time*5.0+(float)j/2.0)*15,
					       0,
					       HGE_DISTORT_POINT_ABS);
		col = (int)((cosf(cur_time*5.0+(i+j)/2.0)+1)*35);
		HGE_distorted_set_color(d, j, i,
					0xff - ((char*)&col)[0],
					0xff - ((char*)&col)[0],
					0xff - ((char*)&col)[0],
					0xff);
				       
	      }
	  break;
	case SDLK_3:
	  {
	    float r,a,dx,dy;
	    float cellw = (d->img->w / (float)(d->grid_w - 1));
	    float cellh = (d->img->h / (float)(d->grid_h - 1));
	    
	    for(i=0;i<d->grid_h;i++)
	      for(j=0;j<d->grid_w;j++)
		{
		  r=sqrtf(powf((float)j-(float)d->grid_h*.5,2)+powf((float)i-(float)d->grid_w*.5,2));
		  a=r*cosf(cur_time*2)*0.1f;
		  dx=sinf(a)*((float)i*cellh-256)+cosf(a)*((float)j*cellw-256);
		  dy=cosf(a)*((float)i*cellh-256)-sinf(a)*((float)j*cellw-256);
		  HGE_distorted_set_displacement(d, j, i, dx, dy, HGE_DISTORT_CENTER);
		}
	  }
	  break;
	default:
	  break;
	}

      t.rotation += (HGE_time_deltaf(cur_time) * rotdelta);
      HGE_distorted_draw_transformed(d, disx, disy, &t);
      cur_time = HGE_time_getf();

      HGE_swapbuffers();

      frames++;
    }
  stop_time = HGE_time_geti();

  printf("%d Frames\n", frames);
  printf("%f Seconds\n", ((float)(stop_time-start_time) / 1000.0));
  printf("%f FPS\n", ((float)frames / ((float)(stop_time-start_time) / 1000.0)));

  HGE_image_free(img);
  HGE_distorted_free(d);

  HGE_quit();

  return 0;
}

