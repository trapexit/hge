#include <stdlib.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_gl_funcs.h"
#include "HGE_mem.h"
#include "HGE_math.h"
#include "HGE_gl.h"
#include "HGE_image.h"
#include "HGE_imap.h"

HGE_typeinfo_t HGE_imap[] = {{sizeof(HGE_imap_t),
			      HGE_imap_init,
			      HGE_imap_clean,
			      NULL,
			      NULL}};

HGE_error_t
HGE_imap_init(void* ptr)
{
  HGE_imap_t* imap = ptr;

  imap->iw = 0;
  imap->ih = 0;
  imap->mw = 0;
  imap->mh = 0;
  imap->images = NULL;

  return HGE_SUCCESS;
}

void
HGE_imap_clean(void* ptr)
{
  HGE_imap_t* imap;

  if((imap = ptr) != NULL)
    {
      int i;
      for(i = 0; i < (imap->iw * imap->ih); i++)
	HGE_unref(imap->images[i]);
      HGE_free(imap->images);
    }
}

void*
HGE_imap_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_imap_t)) != NULL)
    HGE_imap_init(ptr);

  return ptr;
}

void
HGE_imap_free(void* ptr)
{
  HGE_imap_clean(ptr);
  HGE_free(ptr);
}

void
HGE_imap_set_image(HGE_imap_t* imap, HGE_image_t* image, int x, int y)
{
  imap->images[((imap->iw * y) + x)] = image;
}

void*
HGE_imap_get_image(HGE_imap_t* imap, int x, int y)
{
  return imap->images[((imap->iw * y) + x)];
}

int
HGE_imap_set_wh(HGE_imap_t* imap, int w, int h)
{
  HGE_image_t** images;

  images = HGE_realloc(imap->images, HGE_image_t*, (w * h));
  if(images != NULL)
    {
      imap->images = images;
      if((w*h) > (imap->iw * imap->ih))
	{
	  int i;
	  for(i = (imap->iw * imap->ih); i < (w * h); i++)
	    imap->images[i] = NULL;
	}

      imap->iw = w;
      imap->ih = h;
      return HGE_SUCCEEDED;
    }

  return HGE_FAILED;
}

void*
HGE_imap_alloc_from_file(const char* path)
{
  HGE_imap_t* imap;

  if((imap = HGE_imap_alloc()) != NULL)
    {
      if(HGE_imap_make_from_file(imap, path) == HGE_FAILED)
	HGE_free_and_null(imap);
    }

  return imap;
}

void*
HGE_imap_alloc_from_surface(SDL_Surface* surface)
{
  HGE_imap_t* imap;

  if((imap = HGE_imap_alloc()) != NULL)
    {
      if(HGE_imap_make_from_surface(imap, surface) == HGE_FAILED)
	HGE_free_and_null(imap);
    }

  return imap;
}

void*
HGE_imap_new_from_file(const char* path)
{
  HGE_imap_t* imap;

  if((imap = HGE_new(HGE_imap)) != NULL)
    {
      if(HGE_imap_make_from_file(imap, path) == HGE_FAILED)
	HGE_delete_and_null(imap);
    }

  return imap;
}

void*
HGE_imap_new_from_surface(SDL_Surface* surface)
{
  HGE_imap_t* imap;

  if((imap = HGE_new(HGE_imap)) != NULL)
    {
      if(HGE_imap_make_from_surface(imap, surface) == HGE_FAILED)
	HGE_delete_and_null(imap);
    }

  return imap;
}

int
HGE_imap_make_from_file(HGE_imap_t* imap, const char* path)
{
  int rv;
  SDL_Surface* surface;
  

  surface = IMG_Load(path);
  if(surface != NULL)
    {
      rv = HGE_imap_make_from_surface(imap, surface);
      SDL_FreeSurface(surface);
      return rv;
    }
  
  return HGE_FAILED;
}

int
HGE_imap_make_from_surface(HGE_imap_t* imap, SDL_Surface* surface)
{
  int i,j;
  int cx,cy;
  int sw,sh;
  SDL_Rect rect;
  HGE_image_t* img;

  imap->mw = surface->w;
  imap->mh = surface->h;

  sw = HGE_gl_safe_texture_size(surface->w);
  if((sw - surface->w) > (surface->w - (sw>>1)))
    sw>>=1;
  sh = HGE_gl_safe_texture_size(surface->h);
  if((sh - surface->h) > (surface->h - (sh>>1)))
    sh>>=1;
  cx = surface->w / sw;
  if(surface->w != (cx * sw))
    cx++;
  cy = surface->h / sh;
  if(surface->h != (cy * sh))
    cy++;
  if(HGE_imap_set_wh(imap, cx, cy))
    return HGE_FAILED;

  for(j = 0; j < cy; j++)
    {
      for(i = 0; i < cx; i++)
	{
	  rect.x = (i * sw);
	  rect.y = (j * sh);
	  rect.w = (((i * sw) + sw) > surface->w ? surface->w % sw : sw);
	  rect.h = (((j * sh) + sh) > surface->h ? surface->h % sh : sh);

	  img = HGE_image_new_from_surface(surface, &rect);
	  if(img != NULL)
	    HGE_imap_set_image(imap, img, i, j);
	  else
	    {
	      HGE_imap_clean(imap);
	      return HGE_FAILED;
	    }
	}
    }
  
  return HGE_SUCCEEDED;
}

void
HGE_imap_draw(HGE_imap_t* imap, GLint x, GLint y)
{
  int i,j;
  int nx,ny;
  HGE_image_t* img;

  ny = y;
  for(j = 0; j < imap->ih; j++)
    {
      nx = x;
      for(i = 0; i < imap->iw; i++)
	{
	  img = HGE_imap_get_image(imap, i, j);
	  HGE_image_draw(img, nx, ny);
	  nx += img->w;
	}
      ny += img->h;
    }
}

void
HGE_imap_draw_rotate(HGE_imap_t* imap, GLint x, GLint y, GLfloat angle)
{
  int i,j;
  int nx,ny;
  GLint x0,y0;
  GLint x1,y1;
  HGE_image_t* img;

  x1 = (imap->mw >> 1);
  x0 = -x1;
  y1 = (imap->mh >> 1);
  y0 = -y1;

  HGE_glPushMatrix();

  HGE_glTranslatef((float)(x+x1), (float)(y+y1), 0.0);
  HGE_glRotatef(angle, 0.0, 0.0, 1.0);
  
  ny = y0;
  for(j = 0; j < imap->ih; j++)
    {
      nx = x0;
      for(i = 0; i < imap->iw; i++)
	{
	  img = HGE_imap_get_image(imap, 0, j);
	  HGE_image_draw(img, nx, ny);
	  nx += img->w;
	}
      ny += img->h;
    }

  HGE_glPopMatrix();
}

void
HGE_imap_draw_transform(HGE_imap_t* imap, GLint x, GLint y, HGE_transform_t* t)
{
  int i,j;
  int nx,ny;
  GLint x0,y0;
  GLint x1,y1;
  HGE_image_t* img;

  x1 = (imap->mw >> 1);
  x0 = -x1;
  y1 = (imap->mh >> 1);
  y0 = -y1;

  HGE_glPushMatrix();

  if(t->rotation != 0.0)
    HGE_glRotatef(t->rotation, 0.0, 0.0, 1.0);

  if(t->flags & TRANSFORM_H_FLIP)
    HGE_glRotatef(180.0, 0.0, 1.0, 0.0);
  if(t->flags & TRANSFORM_V_FLIP)
    HGE_glRotatef(180.0, 1.0, 0.0, 0.0);

  if(t->scale != 1.0)
    HGE_glScalef(t->scale, t->scale, 1.0);

  ny = y0;
  for(j = 0; j < imap->ih; j++)
    {
      nx = x0;
      for(i = 0; i < imap->iw; i++)
	{
	  img = HGE_imap_get_image(imap, 0, j);
	  HGE_image_draw(img, nx, ny);
	  nx += img->w;
	}
      ny += img->h;
    }
  
  HGE_glPopMatrix();
}
