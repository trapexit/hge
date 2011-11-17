#ifndef _HGE_GL_H_
#define _HGE_GL_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_error.h"

#define HGE_TYPE_UNKNOWN  -1
#define HGE_FORMAT_UNKNOWN -1

#define HGE_clearbuffer() HGE_glClear(GL_COLOR_BUFFER_BIT)
#define HGE_swapbuffers() SDL_GL_SwapBuffers()
#define HGE_swapbuffer()  HGE_swapbuffers()

int    HGE_gl_set_vsync(int n);
void   HGE_gl_enable_textures(void);
void   HGE_gl_disable_textures(void);
void   HGE_gl_perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
GLint  HGE_gl_max_texture_size(void);
GLuint HGE_gl_safe_texture_size(GLuint size);
int    HGE_gl_NPOT_texture_allowed(void);
void   HGE_gl_print_info(void);
void   HGE_gl_find_extensions(void);
int    HGE_gl_extension_supported(const char* extension);

HGE_error_t HGE_gl_load_opengl(void);

#endif
