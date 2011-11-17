#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_gl_funcs.h"
#include "HGE_mem.h"
#include "HGE_config.h"
#include "HGE_gl.h"
#include "HGE_texture.h"

extern HGE_config_t global_config;

void
HGE_texture_replace(GLuint texture, 
		    const GLvoid* data,
		    GLint x, GLint y,
		    GLint w, GLint h,
		    GLenum format, GLenum type)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  HGE_glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
  HGE_glPixelStorei(GL_UNPACK_ROW_LENGTH,  w);
  HGE_glPixelStorei(GL_UNPACK_SKIP_PIXELS, x);
  HGE_glPixelStorei(GL_UNPACK_SKIP_ROWS,   y);
  HGE_glTexSubImage2D(GL_TEXTURE_2D, 0,
	 	      x, y, w, h,
		      format, type, data);
}

GLuint
HGE_texture_make(const GLvoid* data, 
		 GLuint x, GLuint y,
		 GLuint w, GLuint h, 
		 GLenum format, GLenum type, 
		 GLint internal_format)
{
  GLuint texture;
  GLuint sw,sh;

  HGE_glGenTextures(1, &texture);
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  HGE_glPixelStorei(GL_UNPACK_ALIGNMENT,   1);
  HGE_glPixelStorei(GL_UNPACK_ROW_LENGTH,  w);
  HGE_glPixelStorei(GL_UNPACK_SKIP_PIXELS, x);
  HGE_glPixelStorei(GL_UNPACK_SKIP_ROWS,   y);

  sw = HGE_gl_safe_texture_size(w);
  sh = HGE_gl_safe_texture_size(h);
  HGE_glTexImage2D(GL_TEXTURE_2D, 0, 
                   internal_format, 
    	           sw, sh,
	           0, format, type, data);

  if(HGE_glGetError() != GL_NO_ERROR)
    texture = 0;
  
  return texture;
}

void*
HGE_texture_get_pixels(GLuint texture)
{
  GLint w,h;
  void* pixels;

  HGE_texture_get_whi(texture, &w, &h);
  pixels = HGE_allocV(GLubyte, w * h * 4);
  if(pixels != NULL)
    {
      HGE_glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }

  return pixels;
}

GLuint
HGE_texture_dup(GLuint texture)
{
  GLint w,h;
  GLubyte* pixels;
  GLuint new;

  HGE_texture_get_whi(texture, &w, &h);

  pixels = HGE_allocV(GLubyte, w * h * 4);
  if(pixels != NULL)
    {
      HGE_glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      new = HGE_texture_make(pixels, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, global_config.texture_format);
      HGE_free(pixels);
    }

  return new;
}

void
HGE_texture_set_wrap(GLuint texture, GLenum wrap)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);   
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap);
}

void
HGE_texture_set_filter(GLuint texture, GLenum filter)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
  HGE_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter); 
}

void
HGE_texture_get_wrap(GLuint texture, GLint* wrap)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  
  if(wrap != NULL)
    HGE_glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
}

void
HGE_texture_get_filter(GLuint texture, GLint* filter)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  
  if(filter != NULL)
    HGE_glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
}

void
HGE_texture_get_depth(GLuint texture, GLint* depth)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);

  if(depth != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_DEPTH, depth);
}

void
HGE_texture_get_wh(GLuint texture, GLfloat* w, GLfloat* h)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);

  if(w != NULL)
    HGE_glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, w);
  if(h != NULL)
    HGE_glGetTexLevelParameterfv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, h);
}

void
HGE_texture_get_whi(GLuint texture, GLint* w, GLint* h)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);

  if(w != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, w);
  if(h != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, h);
}

void
HGE_texture_get_internal_format(GLuint texture, GLint* internal_format)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  
  if(internal_format != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, internal_format);
}

void
HGE_texture_get_rgba_size(GLuint texture, GLint* r, GLint* g, GLint* b, GLint* a)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);

  if(r != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, r);
  if(g != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_GREEN_SIZE, g);
  if(b != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_BLUE_SIZE, b);
  if(a != NULL)
    HGE_glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, a);
}
