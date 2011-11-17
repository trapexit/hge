#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_gl_funcs.h"
#include "HGE_config.h"
#include "HGE_view.h"

extern HGE_config_t global_config;

void
HGE_view_set(GLint x, GLint y, GLsizei w, GLsizei h)
{
  HGE_glViewport(x, ((global_config.screen->h - h) - y), w, h);
}

void
HGE_view_setv(HGE_view_t* v)
{
  HGE_glViewport(v->x, ((global_config.screen->h - v->h) - v->y), v->w, v->h);
}

void
HGE_view_scale(GLint l, GLint r, GLint t, GLint b)
{
  HGE_glMatrixMode(GL_PROJECTION);
  HGE_glLoadIdentity();
  HGE_glOrtho((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t, -1.0, 1.0);
  HGE_glMatrixMode(GL_MODELVIEW);
  HGE_glLoadIdentity();
  //  HGE_glTranslatef(0.375, 0.375, 0.0);
}

void
HGE_view_scalev(HGE_view_t* v)
{
  HGE_glMatrixMode(GL_PROJECTION);
  HGE_glLoadIdentity();
  HGE_glOrtho((GLdouble)v->x, (GLdouble)v->w, (GLdouble)v->h, (GLdouble)v->y, -1.0, 1.0);
  HGE_glMatrixMode(GL_MODELVIEW);
  HGE_glLoadIdentity();
  //  HGE_glTranslatef(0.375, 0.375, 0.0);
}

void
HGE_view_clip(GLint x, GLint y, GLsizei w, GLsizei h)
{
  HGE_glScissor(x, ((global_config.screen->h - h) - y), w, h);
}

void
HGE_view_clipv(HGE_view_t* v)
{
  HGE_glScissor(v->x, ((global_config.screen->h - v->h) - v->y), v->w, v->h);
}

void
HGE_view_get_clipv(HGE_view_t* v)
{
  HGE_glGetIntegerv(GL_SCISSOR_BOX, (int*)v);
}

void
HGE_view_enable2D(HGE_view_t* v, HGE_view_t* s)
{
  HGE_glEnable(GL_SCISSOR_TEST);

  HGE_glDisable(GL_STENCIL_TEST);
  HGE_glDisable(GL_DEPTH_TEST);
  HGE_glDisable(GL_CULL_FACE);
  HGE_glDisable(GL_LIGHTING);
  HGE_glDisable(GL_POINT_SMOOTH);
  HGE_glDisable(GL_LINE_SMOOTH);
  HGE_glDisable(GL_NORMALIZE);
  HGE_glDisable(GL_DITHER);
  HGE_glDisable(GL_BLEND);

  HGE_glShadeModel(GL_FLAT);
  HGE_glPixelStorei(GL_PACK_ALIGNMENT, 1);
  HGE_glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  HGE_glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

  HGE_view_setv(v);
  HGE_view_scalev(s);
}
