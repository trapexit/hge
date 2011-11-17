#ifndef _HGE_SPRITE_H_
#define _HGE_SPRITE_H_

#include "HGE_list.h"
#include "HGE_slist.h"
#include "HGE_queue.h"
#include "HGE_image.h"
#include "HGE_message.h"

enum
  {
    HGE_SPR_SET_X = 0x0000,
    HGE_SPR_SET_Y,
    HGE_SPR_SET_R,
    HGE_SPR_SET_S,
    HGE_SPR_SET_F,
    HGE_SPR_SET_XY,
    HGE_SPR_SET_RS,
    HGE_SPR_SET_XYR,
    HGE_SPR_SET_XYS,
    HGE_SPR_SET_XYRS,
    HGE_SPR_SET_IMG,
    HGE_SPR_INC_X = 0x0100,
    HGE_SPR_INC_Y,
    HGE_SPR_INC_R,
    HGE_SPR_INC_S,
    HGE_SPR_INC_XY,
    HGE_SPR_INC_RS,
    HGE_SPR_INC_XYR,
    HGE_SPR_INC_XYS,
    HGE_SPR_INC_XYRS,
    HGE_SPR_CINC_X,
    HGE_SPR_CINC_Y,
    HGE_SPR_GOTO_XY,
    HGE_SPR_GOTO_XY2
  };

typedef struct HGE_sprite_t HGE_sprite_t;
struct HGE_sprite_t
{
  GLfloat x;
  GLfloat y;
  HGE_image_t* image;
  HGE_transform_t transform;
  void* data;
  unsigned int flags;
};

extern HGE_typeinfo_t HGE_sprite[];

#define HGE_sprite_set_xy(SPR,X,Y) {((HGE_sprite_t*)SPR)->x = (X); ((HGE_sprite_t*)SPR)->y = (Y);}
#define HGE_sprite_inc_xy(SPR,X,Y) {((HGE_sprite_t*)SPR)->x += (X); ((HGE_sprite_t*)SPR)->y += (Y);}
#define HGE_sprite_set_image(SPR,IMG) ((HGE_sprite_t*)SPR)->image = (IMG)
#define HGE_sprite_set_data(SPR,DATA) ((HGE_sprite_t*)SPR)->data = (DATA)
#define HGE_sprite_set_transform(SPR,T) ((HGE_sprite_t*)SPR)->transform = *((HGE_transform_t*)T)

HGE_error_t HGE_sprite_init(void* ptr);
void        HGE_sprite_clean(void* ptr);
void*       HGE_sprite_alloc(void);
void        HGE_sprite_free(void* ptr);

void* HGE_sprite_alloc_from_file(const char* path, SDL_Rect* rect);
void* HGE_sprite_alloc_from_surface(SDL_Surface* surface, SDL_Rect* rect);
void* HGE_sprite_new_from_file(const char* path, SDL_Rect* rect);
void* HGE_sprite_new_from_surface(SDL_Surface* surface, SDL_Rect* rect);
int   HGE_sprite_make_from_file(HGE_sprite_t* s, const char* path, SDL_Rect* rect);
int   HGE_sprite_make_from_surface(HGE_sprite_t* s, SDL_Surface* surface, SDL_Rect* rect);

void  HGE_sprite_draw(HGE_sprite_t* s);
void  HGE_sprite_drawv(HGE_sprite_t* s, unsigned int n);
void  HGE_sprite_drawpv(HGE_sprite_t** s, unsigned int n);
void  HGE_sprite_draw_slist(HGE_slist_t* sl);
void  HGE_sprite_draw_list(HGE_list_t* l);

int   HGE_sprite_message_filter(HGE_queue_t* inbox, HGE_message_t* msg);
int   HGE_sprite_message_handler(HGE_message_t* msg);

#endif
