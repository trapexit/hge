#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_config.h"
#include "HGE_gl_funcs.h"
#include "HGE_image.h"
#include "HGE_anim.h"
#include "HGE_primitives.h"

extern HGE_config_t global_config;

void
HGE_draw_pointi(GLint x, GLint y)
{
  HGE_glBegin(GL_POINTS);
  HGE_glVertex2i(x,y);
  HGE_glEnd();
}

void
HGE_draw_pointf(GLfloat x, GLfloat y)
{
  HGE_glBegin(GL_POINTS);
  HGE_glVertex2f(x,y);
  HGE_glEnd();
}

void
HGE_draw_pointiv(GLint* p, unsigned int n)
{
  HGE_glEnableClientState(GL_VERTEX_ARRAY);
  HGE_glVertexPointer(2, GL_INT, 0, p);
  HGE_glDrawArrays(GL_POINTS, 0, n);
  HGE_glDisableClientState(GL_VERTEX_ARRAY);
}

void
HGE_draw_pointfv(GLfloat* p, unsigned int n)
{
  HGE_glEnableClientState(GL_VERTEX_ARRAY);
  HGE_glVertexPointer(2, GL_FLOAT, 0, p);
  HGE_glDrawArrays(GL_POINTS, 0, n);
  HGE_glDisableClientState(GL_VERTEX_ARRAY);
}

void
HGE_draw_linei(GLint x0, GLint y0, GLint x1, GLint y1)
{
  HGE_glBegin(GL_LINES);
  HGE_glVertex2i(x0,y0);
  HGE_glVertex2i(x1,y1);
  HGE_glEnd();
}

void
HGE_draw_linef(GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1)
{
  HGE_glBegin(GL_LINES);
  HGE_glVertex2f(x0,y0);
  HGE_glVertex2f(x1,y1);
  HGE_glEnd();
}

void
HGE_draw_lineiv(GLint* l, unsigned int n)
{
  HGE_glEnableClientState(GL_VERTEX_ARRAY);
  HGE_glVertexPointer(2, GL_INT, 0, l);
  HGE_glDrawArrays(GL_LINES, 0, n<<1);
  HGE_glDisableClientState(GL_VERTEX_ARRAY);
}

void
HGE_draw_linefv(GLfloat* l, unsigned int n)
{
  HGE_glEnableClientState(GL_VERTEX_ARRAY);
  HGE_glVertexPointer(2, GL_FLOAT, 0, l);
  HGE_glDrawArrays(GL_LINES, 0, n<<1);
  HGE_glDisableClientState(GL_VERTEX_ARRAY);
}

void
HGE_draw_sdlrect(SDL_Rect* r)
{
  HGE_draw_rect(r->x, r->y, (r->x + r->w), (r->y + r->h));
}

void
HGE_draw_quadv(GLint* q)
{
  HGE_glBegin(GL_QUADS);
  HGE_glVertex2iv(&q[0]);
  HGE_glVertex2iv(&q[2]);
  HGE_glVertex2iv(&q[4]);
  HGE_glVertex2iv(&q[6]);
  HGE_glEnd();
}

void
HGE_draw_quadsv(GLint* q, int n)
{
  HGE_glEnableClientState(GL_VERTEX_ARRAY);
  HGE_glVertexPointer(2, GL_INT, 0, q);
  HGE_glDrawArrays(GL_QUADS, 0, n<<2);
  HGE_glDisableClientState(GL_VERTEX_ARRAY);
}

void
HGE_draw_textured_quadv(GLuint texture, GLint* q)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);

  HGE_draw_quadv(q);
}

void
HGE_draw_textured_quadsv(GLuint* texture, GLint* q, int n)
{
  while(n--)
    {
      HGE_glBindTexture(GL_TEXTURE_2D, *texture);
      HGE_draw_quadv(q);
      
      q += 8;
      texture++;
    }
}

void
HGE_draw_textured_rect(GLuint texture, GLint x, GLint y, GLint w, GLint h)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  HGE_glBegin(GL_QUADS);

  HGE_glTexCoord2i(0,0); HGE_glVertex2i(x,     y);
  HGE_glTexCoord2i(1,0); HGE_glVertex2i(x+w,   y);
  HGE_glTexCoord2i(1,1); HGE_glVertex2i(x+w, y+h);
  HGE_glTexCoord2i(0,1); HGE_glVertex2i(x,   y+h);

  HGE_glEnd();
}

void
HGE_draw_textured_sdlrect(GLuint texture, SDL_Rect* r)
{
  HGE_draw_textured_rect(texture, r->x, r->y, r->w, r->h);
}

void
HGE_draw_textured_square(GLuint texture, GLint x, GLint y, GLint size)
{
  HGE_glBindTexture(GL_TEXTURE_2D, texture);
  HGE_glBegin(GL_QUADS);

  HGE_glTexCoord2i(0,0); HGE_glVertex2i(x,           y);
  HGE_glTexCoord2i(1,0); HGE_glVertex2i(x+size,      y);
  HGE_glTexCoord2i(1,1); HGE_glVertex2i(x+size, y+size);
  HGE_glTexCoord2i(0,1); HGE_glVertex2i(x,      y+size);

  HGE_glEnd();
}

