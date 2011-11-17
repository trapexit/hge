#ifndef _HGE_ANIM_H_
#define _HGE_ANIM_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_basetypes.h"
#include "HGE_parray.h"
#include "HGE_image.h"

enum
  {
    HGE_ANIM_FWD    = 0x00,
    HGE_ANIM_REV    = 0x01,
    HGE_ANIM_REPEAT = 0x04,
    HGE_ANIM_BOUNCE = 0x08,
    HGE_ANIM_RAND   = ~0x0
  };

typedef struct HGE_anim_t HGE_anim_t;
struct HGE_anim_t
{
  HGE_parray_t frames;
  Uint32 cframe;
  unsigned int attr;
  float speed;
  float time;
};

extern HGE_typeinfo_t HGE_anim[];

HGE_error_t HGE_anim_init(void* ptr);
void        HGE_anim_clean(void* ptr);
void*       HGE_anim_alloc(void);
void        HGE_anim_free(void* ptr);

void  HGE_anim_set_frame(HGE_anim_t* anim, HGE_image_t* img, unsigned int frame);
void  HGE_anim_set_frames(HGE_anim_t* anim, HGE_image_t** imgs, unsigned int count);
void  HGE_anim_add_frame(HGE_anim_t* anim, HGE_image_t* img);
void  HGE_anim_add_frames(HGE_anim_t* anim, HGE_image_t** imgs, unsigned int count);
void  HGE_anim_update(HGE_anim_t* anim, float dt);
void  HGE_anim_updateV(HGE_anim_t* anim, unsigned int c, float dt);
void  HGE_anim_updatePV(HGE_anim_t** anim, unsigned int c, float dt);
void  HGE_anim_draw(HGE_anim_t* anim, float x, float y);
void  HGE_anim_set_attr(HGE_anim_t* anim, unsigned int attr);
void  HGE_anim_unset_attr(HGE_anim_t* anim, unsigned int attr);
#endif
