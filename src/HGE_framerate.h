#ifndef _HGE_FRAMERATE_H_
#define _HGE_FRAMERATE_H_

#include "SDL.h"

#define HGE_DEFAULT_FRAMERATE 30

typedef struct HGE_framerate_t HGE_framerate_t;
struct HGE_framerate_t
{
  int    mspf;
  Uint32 last_tick;
};

void HGE_framerate_init(HGE_framerate_t* f);
void HGE_framerate_set(HGE_framerate_t* f, int rate);
void HGE_framerate_delay(HGE_framerate_t* f);
int  HGE_framerate_get(HGE_framerate_t* f);

#endif
