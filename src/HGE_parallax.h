#ifndef _HGE_PARALLAX_H_
#define _HGE_PARALLAX_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_image.h"

typedef struct HGE_parallax_t HGE_parallax_t;
struct HGE_parallax_t
{
  HGE_image_t** img;
  Uint32  width;
  Uint32  height;
  GLfloat x,y;
  GLfloat px,py;
  GLfloat dx;
  GLfloat time_to_wait;
  int     auto_scroll;
};

#endif /* _HGE_PARALLAX_H_ */
