#ifndef _HGE_TYPES_H_
#define _HGE_TYPES_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_error.h"

typedef unsigned int uint;

typedef void (*HGE_func0_t)(void*);
typedef void (*HGE_func1_t)(void*,void*);
typedef void (*HGE_hfunc0_t)(void* k, void* v);
typedef void (*HGE_hfunc1_t)(void* k, void* v, void* d);
typedef int  (*HGE_cfunc_t)(const void*, const void*);
typedef uint (*HGE_hfunc_t)(const void* key);
typedef int  (*HGE_efunc_t)(const void*, const void*);
typedef void (*HGE_ffunc_t)(void*);
typedef int  (*HGE_hffunc0_t)(void* k, void* v);
typedef int  (*HGE_hffunc1_t)(void* k, void* v, void* ud);

typedef struct HGE_v2f_t HGE_v2f_t;
struct HGE_v2f_t
{
  GLfloat x;
  GLfloat y;
};

typedef struct HGE_v3f_t HGE_v3f_t;
struct HGE_v3f_t
{
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

typedef struct HGE_c4ub_v2f_t HGE_c4ub_v2f_t;
struct HGE_c4ub_v2f_t
{
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;
  GLfloat x;
  GLfloat y;
};

typedef struct HGE_c4ub_v3f_t HGE_c4ub_v3f_t;
struct HGE_c4ub_v3f_t
{
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

typedef struct HGE_t2f_v3f_t HGE_t2f_v3f_t;
struct HGE_t2f_v3f_t
{
  GLfloat tx;
  GLfloat ty;
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

typedef struct HGE_t2f_c4ub_v3f_t HGE_t2f_c4ub_v3f_t;
struct HGE_t2f_c4ub_v3f_t
{
  GLfloat tx;
  GLfloat ty;
  GLubyte r;
  GLubyte g;
  GLubyte b;
  GLubyte a;
  GLfloat x;
  GLfloat y;
  GLfloat z;
};

#endif
