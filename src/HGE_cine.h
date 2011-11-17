#ifndef _HGE_CINE_H_
#define _HGE_CINE_H_

#include "HGE_types.h"
#include "HGE_list.h"
#include "HGE_sprite.h"
#include "HGE_loop.h"

enum
  {
    HGE_CINE_PAUSE,
    HGE_CINE_UNPAUSE,
    HGE_CINE_DONE
  };

typedef struct HGE_cine_t HGE_cine_t;
struct HGE_cine_t
{
  Uint32        paused;
  Uint32        start_time;
  HGE_slist_t*  msgs;
  HGE_slist_t*  imgs;
  HGE_slist_t*  sprs;
  HGE_loop_t*   loop;
};

extern HGE_typeinfo_t HGE_cine[];

HGE_error_t HGE_cine_init(void* ptr);
void        HGE_cine_clean(void* ptr);
void*       HGE_cine_alloc(void);
void        HGE_cine_free(void* ptr);

void  HGE_cine_add_sprite(HGE_cine_t* cine, HGE_sprite_t* spr);

void  HGE_cine_pause(HGE_cine_t* cine);
void  HGE_cine_unpause(HGE_cine_t* cine);
void  HGE_cine_run(HGE_cine_t* cine);
void  HGE_cine_run_once(HGE_cine_t* cine);

int   HGE_cine_message_handler(HGE_message_t* msg);


#endif

