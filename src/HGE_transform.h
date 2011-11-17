#ifndef _HGE_TRANSFORM_H_
#define _HGE_TRANSFORM_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_mem.h"

#define TRANSFORM_H_FLIP 0x01
#define TRANSFORM_V_FLIP 0x02

typedef struct HGE_transform_t HGE_transform_t;
struct HGE_transform_t
{
  GLfloat scale;
  GLfloat rotation;
  GLuint  flags;
};

HGE_transform_t* HGE_transform_alloc(void);
void             HGE_transform_free(HGE_transform_t* tf);
void             HGE_transform_init(HGE_transform_t* tf);

#endif
