#include <stdarg.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_gl_funcs.h"
#include "HGE_surface.h"
#include "HGE_font.h"

HGE_typeinfo_t HGE_font[] = {{sizeof(HGE_font_t),
			      HGE_font_init,
			      HGE_font_clean,
			      NULL,
			      NULL}};
HGE_error_t
HGE_font_init(void* ptr)
{
  HGE_font_t* font = ptr;

  font->chars = NULL;
  font->start = '\0';
  font->end   = '\0';

  return HGE_SUCCESS;
}

void
HGE_font_clean(void* ptr)
{
  HGE_font_t* font = ptr;

  if(font != NULL)
    {
      HGE_free(font->chars);
    }
}

void*
HGE_font_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_font_t)) != NULL)
    HGE_font_init(ptr);

  return ptr;
}

void*
HGE_font_alloc_from(const char* path, char start, char end, int w, int h)
{
  HGE_font_t* font;

  font = HGE_font_alloc();
  if(font != NULL)
    {
      if(HGE_font_load(font, path, start, end, w, h))
	{
	  HGE_font_free(font);
	  return NULL;      
	}
    }

  return font;
}

void*
HGE_font_alloc_from_hge(const char* path, char start)
{
  HGE_font_t* font;
  SDL_Surface* s;

  s = IMG_Load(path);
  if(s != NULL)
    {
      font = HGE_font_alloc();
      if(font != NULL)
	{
	  if(HGE_font_load_hge(font, s, start))
	    {
	      HGE_font_free(font);
	      return NULL;      
	    }
	}
      SDL_FreeSurface(s);
    }

  return font;
}

void
HGE_font_free(void* ptr)
{
  HGE_font_clean(ptr);
  HGE_free(ptr);
}

void
HGE_font_resize(HGE_font_t* font, char start, char end)
{
  int i;
  if(font->chars == NULL)
    {
      font->chars = HGE_allocV(HGE_image_t, (end - start + 1));
      for(i = 0; i < (end - start + 1); i++)
	HGE_image_init(&font->chars[i]);
    }
  else
    {
      if(font->start < start)
	start = font->start;
      if(font->end > end)
	end = font->end;

      font->chars = HGE_realloc(font->chars, HGE_image_t, (end-start+1));
      if(start != font->start)
	{
	  for(i = font->start; i <= font->end; i++)
	    {
	      font->chars[start - i] = font->chars[font->start - i];
	    }
	}
    }

  font->start = start;
  font->end   = end;
}

int
HGE_font_load_sfont(HGE_font_t* font, SDL_Surface* s)
{
  int x;
  int chr;
  Uint32 pink;
  SDL_Rect r;
  HGE_image_t img;

  if(SDL_MUSTLOCK(s))
    SDL_LockSurface(s);

  HGE_font_resize(font, '!', '~');

  pink = SDL_MapRGB(s->format, 255, 0, 255);

  x = 0;
  chr = '!';
  while(x < s->w && chr <= '~')
    {
      r.y = 1;
      r.w = 1;
      r.h = s->h - 1;
      while(HGE_surface_get_pixel32(s, x, 0) == pink &&
	    x < s->w)
	x++;
      r.x = x;
      while(HGE_surface_get_pixel32(s, x, 0) != pink &&
	    x < s->w)
	{
	  r.w++;
	  x++;
	}
      HGE_image_make_from_surface(&img, s, &r);
      HGE_font_set_char(font, chr, &img);

      chr++;
    }

  if(SDL_MUSTLOCK(s))
    SDL_UnlockSurface(s);

  return 0;
}

int
HGE_font_load_hge(HGE_font_t* font, SDL_Surface* s, char start)
{
  int x;
  Uint32 c;
  int chr,end;
  SDL_Rect r;
  HGE_image_t img;

  if(SDL_MUSTLOCK(s))
    SDL_LockSurface(s);

  x = 0;
  end = start;
  while(x < s->w)
    {
      c = HGE_surface_get_pixel32(s, x++, 0);
      while(HGE_surface_get_pixel32(s, x, 0) == c)
	x++;

      end++;
    }

  HGE_font_resize(font, start, end);

  x = 0;
  chr = start;
  while(x < s->w)
    {
      r.y = 1;
      r.h = s->h - 1;
      r.w = 1;
      r.x = x;
      c = HGE_surface_get_pixel32(s, x++, 0);
      while(HGE_surface_get_pixel32(s, x, 0) == c)
	{
	  r.w++;
	  x++;
	}

      HGE_image_make_from_surface(&img, s, &r);
      HGE_font_set_char(font, chr, &img);

      chr++;
    }

  if(SDL_MUSTLOCK(s))
    SDL_UnlockSurface(s);

  return 0;
}

int
HGE_font_load(HGE_font_t* font, const char* path, char start, char end, int w, int h)
{
  int i,j;
  float dx,dy;
  char c;
  HGE_image_t img;
  HGE_image_t chr_img;
  SDL_Surface* surface;
  
  surface = IMG_Load(path);
  if(surface == NULL)
    return HGE_FAILED;

  HGE_font_resize(font, start, end);

  HGE_image_init(&img);
  HGE_image_make_from_surface(&img, surface, NULL);

  dx = ((float)w / (float)surface->w) * img.etwp;
  dy = ((float)h / (float)surface->h) * img.ethp;

  c = start;
  chr_img.w = w;
  chr_img.h = h;
  chr_img.bthp = 0.0;
  chr_img.ethp = dy;
  chr_img.texture = img.texture;
  for(j = 0; j < surface->h; j+=h)
    {
      chr_img.btwp = 0.0;
      chr_img.etwp = dx;
      for(i = 0; i < surface->w; i+=w)
	{
	  HGE_font_set_char(font, c++, &chr_img);
	  if(c >= end)
	    goto stop;
	  chr_img.btwp = chr_img.etwp;
	  chr_img.etwp += dx;
	}
      chr_img.bthp = chr_img.ethp;
      chr_img.ethp += dy;
    }

 stop:
  SDL_FreeSurface(surface);

  return HGE_SUCCEEDED;
}

void
HGE_font_set_char(HGE_font_t* font, char c, HGE_image_t* img)
{
  if(c < font->start || c > font->end)
    return;

  font->chars[c - font->start] = *img;
}

float
HGE_font_str_length(HGE_font_t* font, char* str)
{
  float len = 0.0f;
  HGE_image_t* img;

  while(*str && *str != '\n')
    {
      if(*str >= font->start && *str <= font->end)
	img = &font->chars[*str - font->start];
      else
	img = font->chars;

      str++;
      len += img->w;
    }

  return len;
}

float
HGE_font_str_height(HGE_font_t* font, char* str)
{
  float h = font->chars[0].h;

  while(*str)
    {
      if(*str == '\n')
	h += font->chars[0].h;

      str++;
    }

  return h;
}

void
HGE_font_draw(HGE_font_t* font, float x, float y, char* str)
{
  float nx;
  HGE_image_t* img;

  nx = x;
  while(*str)
    {
      if(*str == '\n')
	{
	  y += img->h;
	  nx = x;
	}
      else if(*str >= font->start)
	{
	  img = (*str <= font->end) ? 
	    &font->chars[*str - font->start] : font->chars;
	    
	  HGE_image_draw(img, nx, y);	  
	  nx += img->w;
	}

      str++;
    }
}

void
HGE_font_draw_transformed(HGE_font_t* font, float x, float y, char* str, HGE_transform_t* t)
{
  float nx,ny;
  HGE_image_t* img;

  HGE_glPushMatrix();
  
  HGE_glTranslatef(x, y, 0.0);
  
  if(t->rotation != 0.0)
    HGE_glRotatef(t->rotation, 0.0, 0.0, 1.0); 
  if(t->flags & TRANSFORM_H_FLIP)
    HGE_glRotatef(180.0, 0.0, 1.0, 0.0);
  if(t->flags & TRANSFORM_V_FLIP)
    HGE_glRotatef(180.0, 1.0, 0.0, 0.0);
  if(t->scale != 1.0)
    HGE_glScalef(t->scale, t->scale, 1.0);

  if(t->flags & HGE_FONT_CENTER)
    {
      nx = -HGE_font_str_length(font, str) * 0.5f;
      ny = -HGE_font_str_height(font, str) * 0.5f;
    }
  else
    {
      nx = 0.0f;
      ny = 0.0f;
    }

  while(*str)
    {
      if(*str == '\n')
	{
	  ny += img->h;
	  if(t->flags & HGE_FONT_CENTER)
	    nx = -HGE_font_str_length(font, str+1) * .5;
	  else
	    nx = 0.0f;
	}
      else if(*str >= font->start)
	{
	  img = (*str <= font->end) ? 
	    &font->chars[*str - font->start] : font->chars;

	  HGE_image_draw(img, nx, ny);	  
	  nx += img->w;
	}

      str++;
    }
    

  HGE_glPopMatrix();
}

void
HGE_font_printf(HGE_font_t* font, float x, float y, char* str, ...)
{
  char tmp[256];
  va_list ap;

  va_start(ap, str);
  vsnprintf(tmp, sizeof(tmp), str, ap);
  va_end(ap);
  
  HGE_font_draw(font, x, y, tmp);
}
