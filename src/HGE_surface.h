#ifndef _HGE_SURFACE_H_
#define _HGE_SURFACE_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_gl.h"

GLuint       HGE_surface_guess_gl_type(SDL_Surface* surface);
GLuint       HGE_surface_guess_gl_format(SDL_Surface* surface);
SDL_Surface* HGE_surface_make_rgba8888(SDL_Surface* surface, SDL_Rect* rect);
SDL_Surface* HGE_surface_make_rgba(SDL_Surface* surface, SDL_Rect* rect, Uint32 rmask, Uint32 gmask, Uint32 bmask, Uint32 amask);
GLuint       HGE_surface_make_texture(SDL_Surface* s, SDL_Rect* r);
GLuint       HGE_surface_make_texture2(SDL_Surface* s, SDL_Rect* r, GLuint iformat);
Uint32       HGE_surface_get_pixel32(SDL_Surface* s, Sint32 x, Sint32 y);

#endif /* _HGE_SURFACE_H_ */
