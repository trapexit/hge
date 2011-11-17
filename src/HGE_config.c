#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_config.h"

HGE_config_t global_config =
  {
    SDL_OPENGL,    /* video flags       */
    HGE_TRUE,      /* doublebuffer      */
    0,             /* red size          */
    0,             /* green size        */
    0,             /* blue size         */
    0,             /* alpha size        */
    HGE_FALSE,     /* audio             */
    1,             /* audio_channels    */
    256,           /* audio_buffer_size */
    640,           /* x_res             */
    480,           /* y_res             */
    0,             /* gl_flags          */
    GL_RGBA,       /* texture_format    */
    NULL           /* screen            */
  };

void
HGE_config_copy_config(HGE_config_t* c)
{
  *c = global_config;
}

HGE_config_t*
HGE_config_get_config(void)
{
  return &global_config;
}

void
HGE_config_set_config(HGE_config_t* c)
{
  global_config = *c;
}

void
HGE_config_set_fullscreen(int value)
{
  if(value)
    global_config.videoflags |=  SDL_FULLSCREEN;
  else
    global_config.videoflags &= ~SDL_FULLSCREEN;
}

int
HGE_config_get_fullscreen(void)
{
  if(global_config.screen != NULL)
    return ((global_config.screen->flags | SDL_FULLSCREEN) && HGE_TRUE);
  else
    return ((global_config.videoflags | SDL_FULLSCREEN) && HGE_TRUE);
}

void
HGE_config_set_no_frame(int value)
{
  if(value)
    global_config.videoflags |=  SDL_NOFRAME;
  else
    global_config.videoflags &= ~SDL_NOFRAME;
}

int
HGE_config_get_no_frame(void)
{
  if(global_config.screen != NULL)
    return ((global_config.screen->flags | SDL_NOFRAME) && HGE_TRUE);
  else
    return ((global_config.videoflags | SDL_NOFRAME) && HGE_TRUE);
}

void
HGE_config_set_resizable(int value)
{
  if(value)
    global_config.videoflags |=  SDL_RESIZABLE;
  else
    global_config.videoflags &= ~SDL_RESIZABLE;
}

int
HGE_config_get_resizable(void)
{
  if(global_config.screen != NULL)
    return ((global_config.screen->flags | SDL_RESIZABLE) && HGE_TRUE);
  else
    return ((global_config.videoflags | SDL_RESIZABLE) && HGE_TRUE);
}

void
HGE_config_set_audio(int value)
{
  global_config.audio = (value ? HGE_TRUE : HGE_FALSE);
}

void
HGE_config_set_audio_channels(int value)
{
  global_config.audio_channels = value;
}

void
HGE_config_set_audio_buffer_size(int value)
{
  global_config.audio_buffer_size = value;
}

void
HGE_config_set_resolution(int x_res, int y_res)
{
  global_config.x_res = x_res;
  global_config.y_res = y_res;
}

void
HGE_config_get_resolution(int* x_res, int* y_res)
{
  if(global_config.screen != NULL)
    {
      *x_res = global_config.screen->w;
      *y_res = global_config.screen->h;
    }
  else
    {
      *x_res = global_config.x_res;
      *y_res = global_config.y_res;
    }
}

SDL_Surface*
HGE_config_get_screen(void)
{
  return global_config.screen;
}

void
HGE_config_set_texture_format(GLuint format)
{
  global_config.texture_format = format;
}

GLuint
HGE_config_get_texture_formate(void)
{
  return global_config.texture_format;
}

void
HGE_config_set_doublebuffer(int value)
{
  global_config.doublebuffer = (value ? HGE_TRUE : HGE_FALSE);
}

int
HGE_config_get_doublebuffer(void)
{
  return global_config.doublebuffer;
}

void
HGE_config_set_rgba(int r, int g, int b, int a)
{
  global_config.red_size   = r;
  global_config.green_size = g;
  global_config.blue_size  = b;
  global_config.alpha_size = a;  
}

void
HGE_config_get_rgba(int* r, int* g, int* b, int* a)
{
  if(r != NULL)
    *r = global_config.red_size;
  if(g != NULL)
    *g = global_config.green_size;
  if(b != NULL)
    *b = global_config.blue_size;
  if(a != NULL)
    *a = global_config.alpha_size;
}
