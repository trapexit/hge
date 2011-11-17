#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_gl_funcs.h"
#include "HGE_mem.h"
#include "HGE_gl.h"
#include "HGE_config.h"
#include "HGE_texture.h"
#include "HGE_surface.h"
#include "HGE_image.h"
#include "HGE_trash_stack.h"

extern HGE_config_t global_config;

HGE_typeinfo_t HGE_image[] = {{sizeof(HGE_image_t),
			       HGE_image_init,
			       HGE_image_clean,
			       NULL,
			       NULL}};

HGE_error_t
HGE_image_init(void* ptr)
{
  HGE_image_t* image = ptr;

  image->texture = 0;
  image->w       = 0.0f;
  image->h       = 0.0f;
  image->btwp    = 0.0f;
  image->bthp    = 0.0f;
  image->etwp    = 0.0f;
  image->ethp    = 0.0f;

  return HGE_SUCCESS;
}

void
HGE_image_clean(void* ptr)
{
  
}

void*
HGE_image_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_image_t)) != NULL)
    HGE_image_init(ptr);

  return ptr;
}

void
HGE_image_free(void* ptr)
{
  HGE_image_clean(ptr);
  HGE_free(ptr);
}

void*
HGE_image_alloc_from_surface(SDL_Surface* surface, SDL_Rect* rect)
{
  HGE_image_t* img;

  if((img = HGE_image_alloc()) != NULL)
    {
      if(HGE_image_make_from_surface(img, surface, rect))
	{
	  HGE_image_free(img);
	  return NULL;
	}
    }

  return img;
}

void*
HGE_image_alloc_from_file(const char* path, SDL_Rect* rect)
{
  HGE_image_t* img;

  if((img = HGE_image_alloc()) != NULL)
    {
      if(HGE_image_make_from_file(img, path, rect))
	{
	  HGE_image_free(img);
	  return NULL;
	}
    }

  return img;
}

void*
HGE_image_new_from_surface(SDL_Surface* surface, SDL_Rect* rect)
{
  HGE_image_t* img;

  if((img = HGE_new(HGE_image)) != NULL)
    {
      if(HGE_image_make_from_surface(img, surface, rect))
	{
	  return HGE_unref(img);
	}
    }

  return img;
}

void*
HGE_image_new_from_file(const char* path, SDL_Rect* rect)
{
  HGE_image_t* img;

  if((img = HGE_new(HGE_image)) != NULL)
    {
      if(HGE_image_make_from_file(img, path, rect))
	{
	  return HGE_unref(img);
	}
    }
	
  return img;
}

HGE_error_t
HGE_image_make_from_file(HGE_image_t* img, const char* path, SDL_Rect* rect)
{
  HGE_error_t rv;
  SDL_Surface* sf;

  rv = HGE_FAILED;
  sf = IMG_Load(path);
  if(sf != NULL)
    {
      rv = HGE_image_make_from_surface(img, sf, rect);
      SDL_FreeSurface(sf);
    }

  return rv;
}

HGE_error_t
HGE_image_make_from_surface2(HGE_image_t* img, SDL_Surface* surface, SDL_Rect* rect, GLuint format)
{
  int w,h;
  GLfloat tw,th;

  if(rect == NULL)
    {w = surface->w; h = surface->h;}
  else
    {w = rect->w;    h = rect->h;}

  img->texture = HGE_surface_make_texture2(surface, rect, format);
  HGE_texture_get_wh(img->texture, &tw, &th);

  img->w    = (GLfloat)w;
  img->h    = (GLfloat)h;
  img->btwp = 0.0f;
  img->bthp = 0.0f;
  img->etwp = (img->w/tw);
  img->ethp = (img->h/th);

  return (img->texture ? HGE_SUCCEEDED : HGE_FAILED);
}

HGE_error_t
HGE_image_make_from_surface(HGE_image_t* img, SDL_Surface* surface, SDL_Rect* rect)
{
  return HGE_image_make_from_surface2(img, surface, rect, global_config.texture_format);
}

void
HGE_image_make_tiled(HGE_image_t* img, GLfloat w, GLfloat h)
{
  GLfloat tw,th;

  HGE_texture_get_wh(img->texture, &tw, &th);

  img->w = w;
  img->h = h;
  img->btwp = 0.0;
  img->bthp = 0.0;
  img->etwp = (w / tw);
  img->ethp = (h / th);
  HGE_texture_set_wrap(img->texture, GL_REPEAT);
}

void
HGE_image_draw(HGE_image_t* i, GLfloat x, GLfloat y)
{
  HGE_glBindTexture(GL_TEXTURE_2D, i->texture);

  HGE_glBegin(GL_QUADS);

  HGE_glTexCoord2f(i->btwp, i->bthp); HGE_glVertex2f(x,           y);
  HGE_glTexCoord2f(i->etwp, i->bthp); HGE_glVertex2f(x+i->w,      y);
  HGE_glTexCoord2f(i->etwp, i->ethp); HGE_glVertex2f(x+i->w, y+i->h);
  HGE_glTexCoord2f(i->btwp, i->ethp); HGE_glVertex2f(x,      y+i->h);

  HGE_glEnd();
}

void
HGE_image_draw_scaled(HGE_image_t* i, GLfloat x, GLfloat y, GLfloat scale)
{
  GLfloat x0,y0;
  GLfloat x1,y1;

  x1 = (i->w * 0.5f);
  x0 = -x1;
  y1 = (i->h * 0.5f);
  y0 = -y1;

  HGE_glPushMatrix();

  HGE_glTranslatef((x+x1), (y+y1), 0.0);
  HGE_glScalef(scale, scale, 1.0);

  HGE_glBindTexture(GL_TEXTURE_2D, i->texture);
  HGE_glBegin(GL_QUADS);

  HGE_glTexCoord2f(i->btwp, i->bthp); HGE_glVertex2f(x0,y0);
  HGE_glTexCoord2f(i->etwp, i->bthp); HGE_glVertex2f(x1,y0);
  HGE_glTexCoord2f(i->etwp, i->ethp); HGE_glVertex2f(x1,y1);
  HGE_glTexCoord2f(i->btwp, i->ethp); HGE_glVertex2f(x0,y1);

  HGE_glEnd();
  
  HGE_glPopMatrix();
}

void
HGE_image_draw_rotated(HGE_image_t* i, GLfloat x, GLfloat y, GLfloat deg)
{
  GLfloat x0,y0;
  GLfloat x1,y1;

  x1 = (i->w * 0.5f);
  x0 = -x1;
  y1 = (i->h * 0.5f);
  y0 = -y1;

  HGE_glPushMatrix();

  HGE_glTranslatef((x+x1), (y+y1), 0.0);
  HGE_glRotatef(deg, 0.0, 0.0, 1.0);
  
  HGE_glBegin(GL_QUADS);

  HGE_glTexCoord2f(i->btwp, i->bthp); HGE_glVertex2f(x0,y0);
  HGE_glTexCoord2f(i->etwp, i->bthp); HGE_glVertex2f(x1,y0);
  HGE_glTexCoord2f(i->etwp, i->ethp); HGE_glVertex2f(x1,y1);
  HGE_glTexCoord2f(i->btwp, i->ethp); HGE_glVertex2f(x0,y1);

  HGE_glEnd();

  HGE_glPopMatrix();
}

void
HGE_image_draw_transformed(HGE_image_t* i, GLfloat x, GLfloat y, HGE_transform_t* t)
{
  GLfloat x0,y0;
  GLfloat x1,y1;

  x1 = (i->w * .5);
  x0 = -x1;
  y1 = (i->h * .5);
  y0 = -y1;

  HGE_glPushMatrix();

  HGE_glTranslatef((x+x1), (y+y1), 0.0);

  if(t->rotation != 0.0)
    HGE_glRotatef(t->rotation, 0.0, 0.0, 1.0);

  if(t->flags & TRANSFORM_H_FLIP)
    HGE_glRotatef(180.0, 0.0, 1.0, 0.0);
  if(t->flags & TRANSFORM_V_FLIP)
    HGE_glRotatef(180.0, 1.0, 0.0, 0.0);

  if(t->scale != 1.0)
    HGE_glScalef(t->scale, t->scale, 1.0);

  HGE_glBindTexture(GL_TEXTURE_2D, i->texture);
  HGE_glBegin(GL_QUADS);

  HGE_glTexCoord2f(i->btwp, i->bthp); HGE_glVertex2f(x0,y0);
  HGE_glTexCoord2f(i->etwp, i->bthp); HGE_glVertex2f(x1,y0);
  HGE_glTexCoord2f(i->etwp, i->ethp); HGE_glVertex2f(x1,y1);
  HGE_glTexCoord2f(i->btwp, i->ethp); HGE_glVertex2f(x0,y1);

  HGE_glEnd();

  HGE_glPopMatrix();
}

