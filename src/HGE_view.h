#ifndef _HGE_VIEW_H_
#define _HGE_VIEW_H_

#include "SDL.h"
#include "SDL_opengl.h"

typedef struct HGE_view_t HGE_view_t;
struct HGE_view_t
{
  GLint x;
  GLint y;
  GLsizei w;
  GLsizei h;
};

void HGE_view_set(GLint x, GLint y, GLsizei w, GLsizei h);
void HGE_view_setv(HGE_view_t* v);
void HGE_view_scale(GLint l, GLint r, GLint t, GLint b);
void HGE_view_scalev(HGE_view_t* v);
void HGE_view_clip(GLint x, GLint y, GLsizei w, GLsizei h);
void HGE_view_clipv(HGE_view_t* v);
void HGE_view_get_clipv(HGE_view_t* v);
void HGE_view_enable2D(HGE_view_t* v, HGE_view_t* s);

#endif
