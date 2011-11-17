#include "SDL.h"
#include "HGE_defines.h"
#include "HGE_config.h"
#include "HGE_texture.h"
#include "HGE_surface.h"

extern HGE_config_t global_config;

Uint32
HGE_surface_get_pixel32(SDL_Surface* s, Sint32 x, Sint32 y)
{
  return *((Uint32*)s->pixels + (y * s->pitch>>2) + x);
}

SDL_Surface*
HGE_surface_make_rgba(SDL_Surface* surface, SDL_Rect* rect,
		      Uint32 rmask, Uint32 gmask, Uint32 bmask, Uint32 amask)
{
  int w,h;
  SDL_Surface* ns;
  
  if(rect != NULL)
    {w = rect->w;    h = rect->h;}
  else
    {w = surface->w; h = surface->h;}

  ns = SDL_CreateRGBSurface(SDL_SWSURFACE,
			    w, h, 32, 
			    rmask, gmask, bmask, amask);

  if(ns != NULL)
    {
      SDL_SetAlpha(surface, 0, 0);
      SDL_BlitSurface(surface, rect, ns, NULL);
    }
  
  return ns;
}

SDL_Surface*
HGE_surface_make_rgba8888(SDL_Surface* surface, SDL_Rect* rect)
{
  return HGE_surface_make_rgba(surface, rect,
			       HGE_RMASK_8_8_8_8, 
			       HGE_GMASK_8_8_8_8,
			       HGE_BMASK_8_8_8_8,
			       HGE_AMASK_8_8_8_8);
}

GLuint
HGE_surface_guess_gl_format(SDL_Surface* surface)
{
  SDL_PixelFormat* format = surface->format;

  if(format->Rshift < format->Bshift)
    return (format->Amask ? GL_RGBA : GL_RGB);
#if defined GL_BGRA && defined GL_BGR
  else if(format->Rshift > format->Bshift)
    return (format->Amask ? GL_BGRA : GL_BGR);
#endif

  return -1;
}

GLuint
HGE_surface_guess_gl_type(SDL_Surface* surface)
{
  SDL_PixelFormat* format = surface->format;

  /* This is all to optimize loading images which OpenGL can understand */
  /* natively... however it seems to segv on my machine at times in     */
  /* glTexImage2D if i just pass it along without using SDL to conver it*/
  return -1;

  switch(format->BytesPerPixel)
    {
    default:
    case 1: 
      break;
    case 2:
      switch(format->Bmask)
	{
	case HGE_BMASK_4_4_4_4:
	  return HGE_UNSIGNED_SHORT_4_4_4_4;
	case HGE_BMASK_4_4_4_4_REV:
	  return HGE_UNSIGNED_SHORT_4_4_4_4_REV;
	case HGE_BMASK_5_5_5_1:
	  return HGE_UNSIGNED_SHORT_5_5_5_1;
	case HGE_BMASK_1_5_5_5_REV:
	  return HGE_UNSIGNED_SHORT_1_5_5_5_REV;
	case HGE_BMASK_5_6_5:
	  return HGE_UNSIGNED_SHORT_5_6_5;
	case HGE_BMASK_5_6_5_REV:
	  return HGE_UNSIGNED_SHORT_5_6_5_REV;
	default:
	  break;
	}
      break;
    case 3:
      break;
    case 4:
      switch(format->Rmask)
	{
	case HGE_RMASK_8_8_8_8:
	  return HGE_UNSIGNED_INT_8_8_8_8;
	case HGE_RMASK_8_8_8_8_REV:
	  return HGE_UNSIGNED_INT_8_8_8_8_REV;
	case HGE_RMASK_10_10_10_2:
	  return HGE_UNSIGNED_INT_10_10_10_2;
	case HGE_RMASK_2_10_10_10_REV:
	  return HGE_UNSIGNED_INT_2_10_10_10_REV;
	default:
	  break;
	}
      break;
    }

  return -1;
}

GLuint
HGE_surface_make_texture2(SDL_Surface* s, SDL_Rect* r, GLuint iformat)
{
  GLint type;
  GLint format;
  GLuint x,y,w,h;
  GLuint texture;

  if(r != NULL)
    {x = r->x; y = r->y; w = r->w; h = r->h;}
  else
    {x = y = 0; w = s->w; h = s->h;}

  type   = HGE_surface_guess_gl_type(s);
  format = HGE_surface_guess_gl_format(s);
  if(type   == HGE_TYPE_UNKNOWN ||
     format == HGE_FORMAT_UNKNOWN)
    {
      s = HGE_surface_make_rgba8888(s, r);
      x = y = 0;
      format = GL_RGBA;
      type   = GL_UNSIGNED_BYTE;
    }
  else
    s->refcount++;


  texture = HGE_texture_make(s->pixels,
			     x, y, w, h,
			     format, type,
			     iformat);
			     
  SDL_FreeSurface(s);

  return texture;
}

GLuint
HGE_surface_make_texture(SDL_Surface* s, SDL_Rect* r)
{
  return HGE_surface_make_texture2(s, r, global_config.texture_format);
}
