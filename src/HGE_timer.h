#ifndef _HGE_TIMER_H_
#define _HGE_TIMER_H_

#include "SDL.h"

typedef struct HGE_timer_t HGE_timer_t;
struct HGE_timer_t
{
  Uint32 start_tick;
  Uint32 length;
};

void*        HGE_timer_alloc(void);
void         HGE_timer_init(void* ptr);
void         HGE_timer_free(void* ptr);
void         HGE_timer_set(HGE_timer_t* t, Uint32 length);
int          HGE_timer_done(HGE_timer_t* t);
Sint32       HGE_timer_time_left(HGE_timer_t* t);
Sint32       HGE_timer_time_past(HGE_timer_t* t);
int          HGE_timer_done_given(HGE_timer_t* t, Uint32 start_tick);
Sint32       HGE_timer_time_left_given(HGE_timer_t* t, Uint32 start_tick);
Sint32       HGE_timer_time_past_given(HGE_timer_t* t, Uint32 start_tick);

#endif
