#ifndef _HGE_PRIMITIVES_H_
#define _HGE_PRIMITIVES_H_

#include "SDL.h"
#include "SDL_opengl.h"

#define HGE_draw_rect(x0,y0,x1,y1) HGE_glRecti(x0,y0,x1,y1)

void HGE_draw_pointi(GLint x, GLint y);
void HGE_draw_pointf(GLfloat x, GLfloat y);
void HGE_draw_pointiv(GLint* p, unsigned int n);
void HGE_draw_pointfv(GLfloat* p, unsigned int n);

void HGE_draw_linei(GLint x0, GLint y0, GLint x1, GLint y1);
void HGE_draw_linef(GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1);
void HGE_draw_lineiv(GLint* l, unsigned int n);
void HGE_draw_linefv(GLfloat* l, unsigned int n);

void HGE_draw_sdlrect(SDL_Rect* r);
void HGE_draw_quadv(GLint* p);
void HGE_draw_quadsv(GLint* p, int n);
void HGE_draw_textured_quad(GLuint texture, GLint x, GLint y, GLint w, GLint h);
void HGE_draw_textured_square(GLuint texture, GLint x, GLint y, GLint size);

#endif
