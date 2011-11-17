#ifndef _HGE_TRIGGER_H_
#define _HGE_TRIGGER_H_

#include <stdarg.h>
#include "SDL.h"
#include "HGE_timer.h"
#include "HGE_message.h"

enum
  {
    HGE_TRIGGER_ENABLE,
    HGE_TRIGGER_DISABLE
  };

enum
  {
    HGE_MOUSE_TGR        = 0x0000,
    HGE_MOUSE_FOCUS      = 0x0000,
    HGE_MOUSE_UNFOCUS    = 0x0001,
    HGE_MOUSE_BUTTONDOWN = 0x0002,
    HGE_MOUSE_BUTTONUP   = 0x0003,

    HGE_ACTIVE_TGR       = 0x0100,
    HGE_ACTIVE_GAIN      = 0x0100,
    HGE_ACTIVE_LOST      = 0x0101,

    HGE_QUIT_TGR         = 0xFFFF
  };

typedef struct HGE_trigger_t HGE_trigger_t;
struct HGE_trigger_t
{
  int            (*handler)(void*, const void*);
  int            count;
  int            active;
  HGE_message_t* msg;
};

typedef struct HGE_trigger_mouse_t HGE_trigger_mouse_t;
struct HGE_trigger_mouse_t
{
  int            (*handler)(void*, const void*);
  int            count;
  int            active;
  HGE_message_t* msg;

  SDL_Rect box;
  Uint8 buttons;
};

typedef struct HGE_trigger_active_t HGE_trigger_active_t;
struct HGE_trigger_active_t
{
  int            (*handler)(void*, const void*);
  int            count;
  int            active;
  HGE_message_t* msg;
  
  Uint8 gain;
  Uint8 state;
};

void HGE_trigger_init_system(void);
void HGE_trigger_clean_system(void);

void* HGE_trigger_add(int type, HGE_message_t* msg, int count, ...);

int  HGE_trigger_message_handler(HGE_message_t* msg);
#endif /* _HGE_TRIGGER_H_ */
