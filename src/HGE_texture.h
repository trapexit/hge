#ifndef _HGE_TEXTURE_H_
#define _HGE_TEXTURE_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_typeinfo.h"

struct HGE_texture_t
{
  
};

GLuint  HGE_texture_make(const GLvoid* data, GLuint x, GLuint y, GLuint w, GLuint h, GLenum format, GLenum type, GLint internal_format);
GLuint  HGE_texture_dup(GLuint texture);
void*   HGE_texture_get_pixels(GLuint texture);
void    HGE_texture_set_wrap(GLuint texture, GLenum wrap);
void    HGE_texture_set_filter(GLuint texture, GLenum filter);
void    HGE_texture_get_wrap(GLuint texture, GLint* wrap);
void    HGE_texture_get_filter(GLuint texture, GLint* filter);
void    HGE_texture_get_depth(GLuint texture, GLint* depth);
GLfloat HGE_texture_get_width(GLuint texture);
GLfloat HGE_texture_get_height(GLuint texture);
void    HGE_texture_get_wh(GLuint texture, GLfloat* w, GLfloat* h);
void    HGE_texture_get_whi(GLuint texture, GLint* w, GLint* h);
void    HGE_texture_get_internal_format(GLuint texture, GLint* internal_format);
void    HGE_texture_get_rgba_size(GLuint texture, GLint* r, GLint* g, GLint* b, GLint* a);

#endif /* _HGE_TEXTURE_H_ */
