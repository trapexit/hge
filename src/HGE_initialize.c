#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_mixer.h"
#include "HGE_defines.h"
#include "HGE_gl_funcs.h"
#include "HGE_config.h"
#include "HGE_view.h"
#include "HGE_misc.h"
#include "HGE_gl.h"
#include "HGE_list.h"
#include "HGE_slist.h"
#include "HGE_hash.h"
#include "HGE_cine.h"
#include "HGE_image.h"
#include "HGE_sprite.h"
#include "HGE_trigger.h"
#include "HGE_postoffice.h"
#include "HGE_initialize.h"

void
HGE_init(void)
{
  static int inited = HGE_FALSE;

  srand(time(NULL));
  if(inited == HGE_FALSE)
    {
      HGE_trigger_init_system();
      HGE_postoffice_init_system();
      inited = HGE_TRUE;
    }
}

void
HGE_quit(void)
{
  HGE_postoffice_clean_system();
  HGE_trigger_clean_system();

  SDL_Quit();
}

int
HGE_initialize_timer(void)
{
  if(SDL_WasInit(SDL_INIT_TIMER))
    SDL_QuitSubSystem(SDL_INIT_TIMER);
  
  SDL_InitSubSystem(SDL_INIT_TIMER);

  return (SDL_WasInit(SDL_INIT_TIMER) == SDL_INIT_TIMER);
}

int
HGE_initialize_video(void)
{
  int rv;
  int bpp;
  Uint32 video_flags;
  SDL_bool done;
  int driver_count;
  char driver_name[32];
  HGE_view_t view;
  HGE_config_t* config;
  const SDL_VideoInfo* video_info;
  const static char* video_drivers[] = {"x11", "dga", "nanox","fbcon","directfb","ps2gs",
					"ggi","vgl","svgalib","directx","windib","bwindow",
					"toolbox","DSp","Quartz","CGX","photon","epoc","aalib", NULL};

  HGE_init();

  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_LoadLibrary(NULL);
  if(HGE_gl_load_opengl() == HGE_FAILURE)
    return HGE_FAILURE;
  
  config = HGE_config_get_config();

  driver_count = 0;
  video_flags = config->videoflags;
  done = HGE_FALSE;
  while(done == HGE_FALSE)
    {
      if(SDL_WasInit(SDL_INIT_VIDEO))
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
      
      rv = SDL_InitSubSystem(SDL_INIT_VIDEO);
      video_info = SDL_GetVideoInfo();
      if(rv && !video_info)
	{
	  if(video_drivers[driver_count] != NULL)
	    {
	      snprintf(driver_name, 32, "SDL_VIDEODRIVER=%s", video_drivers[driver_count]);
	      putenv(driver_name);
	      driver_count++;
	      continue;
	    }
	  
	  fprintf(stderr, "Error: %s\n", SDL_GetError());
	  done = HGE_TRUE;
	  continue;
	}
      
      SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     config->red_size);
      SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   config->green_size);
      SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    config->blue_size);
      SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   config->alpha_size);
      SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, config->doublebuffer);

      if(config->videoflags & SDL_FULLSCREEN)
	video_flags |= SDL_FULLSCREEN;

      bpp = SDL_VideoModeOK(config->x_res, config->y_res, video_info->vfmt->BitsPerPixel, video_flags);
      if(bpp)
	config->screen = SDL_SetVideoMode(config->x_res, config->y_res, 0, video_flags);
      else
	config->screen = NULL;

      if(config->screen != NULL)
	{
	  if(config->screen->flags & SDL_FULLSCREEN)
	    config->videoflags |= SDL_FULLSCREEN;

	  HGE_glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	  view.x = 0;
	  view.y = 0;
	  view.w = config->screen->w;
	  view.h = config->screen->h;
	  HGE_view_enable2D(&view, &view);
	  HGE_view_clipv(&view);
	  HGE_gl_enable_textures();
          HGE_gl_find_extensions();
	  SDL_GL_GetAttribute(SDL_GL_RED_SIZE,     &config->red_size);
	  SDL_GL_GetAttribute(SDL_GL_GREEN_SIZE,   &config->green_size);
	  SDL_GL_GetAttribute(SDL_GL_BLUE_SIZE,    &config->blue_size);
	  SDL_GL_GetAttribute(SDL_GL_ALPHA_SIZE,   &config->alpha_size);
	  SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &config->doublebuffer);
	  
	  done = HGE_TRUE;
	}
      else
	{
	  SDL_QuitSubSystem(SDL_INIT_VIDEO);
	  
	  if(video_drivers[driver_count] == NULL)
	    {
	      if(config->videoflags & SDL_FULLSCREEN)
		{
		  config->videoflags &= ~SDL_FULLSCREEN;
		  driver_count = 0;
		}
	    }
	  
	  if(video_drivers[driver_count] != NULL)
	    {
	      snprintf(driver_name, 32, "SDL_VIDEODRIVER=%s", video_drivers[driver_count]);
	      putenv(driver_name);
	      driver_count++;
	      continue;
	    }
	     
	  fprintf(stderr, "Error: %s\n", SDL_GetError());
	  done = HGE_TRUE;
	  continue;
	}
    }

  return (SDL_WasInit(SDL_INIT_VIDEO) == SDL_INIT_VIDEO);
}

int
HGE_initialize_audio(void)
{
  int rv;
  SDL_bool done;
  int driver_count;
  char driver_name[32];
  HGE_config_t* config;
  const static char* audio_drivers[] = {"alsa", "esd", "artsc", "dma", "dsp", "audio",
					"nas", "openbsd", "AL", "paud", "sndmgr", "dsound",
					"baudio", "AHI", NULL};

  config = HGE_config_get_config();

  if(SDL_AudioDriverName(driver_name, 32))
    {
      if(!strcasecmp(driver_name, "dsp"))
	snprintf(driver_name, 32, "dma");
    }

  driver_count = 0;
  done = HGE_FALSE;
  while(done == HGE_FALSE)
    {
      if(config->audio == HGE_TRUE)
	{
	  if(SDL_WasInit(SDL_INIT_AUDIO))
	    SDL_QuitSubSystem(SDL_INIT_AUDIO);
	  
	  rv = SDL_InitSubSystem(SDL_INIT_AUDIO);
	  if(rv)
	    {
	      if(audio_drivers[driver_count] != NULL)
		{
		  snprintf(driver_name, 32, "SDL_AUDIODRIVER=%s", audio_drivers[driver_count]);
		  putenv(driver_name);
		  driver_count++;
		  continue;
		}
	      
	      fprintf(stderr, "Warning: %s : Turning audio off\n", SDL_GetError());
	      config->audio = HGE_FALSE;
	    }
	  else
	    {
	      rv = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, config->audio_channels, config->audio_buffer_size);
	      if(rv)
		{
		  SDL_QuitSubSystem(SDL_INIT_AUDIO);
		  
		  if(audio_drivers[driver_count] == NULL)
		    {
		      if(config->audio_channels > 0)
			{
			  config->audio_channels--;
			  driver_count = 0;
			}
		    }

		  if(audio_drivers[driver_count] != NULL)
		    {
		      snprintf(driver_name, 32, "SDL_AUDIODRIVER=%s", audio_drivers[driver_count]);
		      putenv(driver_name);
		      driver_count++;
		      continue;
		    }

		  fprintf(stderr, "Warning: %s : Turning audio off\n", Mix_GetError());
		  config->audio = HGE_FALSE;
		}
	    }
	}
      done = HGE_TRUE;
    }

  return (SDL_WasInit(SDL_INIT_AUDIO) == SDL_INIT_AUDIO);
}
