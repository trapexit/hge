#include <stdarg.h>
#include "SDL.h"
#include "HGE_mem.h"
#include "HGE_types.h"
#include "HGE_defines.h"
#include "HGE_list.h"
#include "HGE_slist.h"
#include "HGE_queue.h"
#include "HGE_message.h"
#include "HGE_postoffice.h"
#include "HGE_trigger.h"

static HGE_slist_t* SDL_EV_TGRS[SDL_NUMEVENTS];

/* static local functions */
static int
HGE_trigger_event_filter(const SDL_Event* event)
{
  HGE_slist_t* node;
  HGE_trigger_t* tgr;

  node = SDL_EV_TGRS[event->type];
  while(node != NULL)
    {
      tgr = node->data;
      
      if(tgr->active != HGE_FALSE)
	{
	  if(tgr->handler(tgr, event))
	    printf("triggered: %d\n", SDL_GetTicks());
	  /* 
	     if(tgr->msg != NULL)
  	       HGE_postoffice_send_msg(0, HGE_ref(tgr->msg)) 
	  */
	}

      node = node->next;
    }

  return 1;
}

/* trigger functions */
static int
HGE_trigger_mouse_focus(void* t, const void* data)
{
  const HGE_trigger_mouse_t* tgr = t;
  const SDL_MouseMotionEvent* event = data;

  if(event->state == tgr->buttons)
    return ((event->x >= tgr->box.x) &&
	    (event->x <  tgr->box.w) &&
	    (event->y >= tgr->box.y) &&
	    (event->y <  tgr->box.h));

  return HGE_FALSE;
}

static int
HGE_trigger_mouse_unfocus(void* t, const void* data)
{
  const HGE_trigger_mouse_t* tgr = t;
  const SDL_MouseMotionEvent* event = data;

  if(event->state == tgr->buttons)
    return ((event->x <  tgr->box.x) ||
	    (event->x >= tgr->box.w) ||
	    (event->y <  tgr->box.y) ||
	    (event->y >= tgr->box.h));

  return HGE_FALSE;
}

static int
HGE_trigger_active(void* t, const void* data)
{
  const HGE_trigger_active_t* tgr = t;
  const SDL_ActiveEvent* event = data;

  return ((tgr->gain  == event->gain) &&
	  (tgr->state == event->state));
}

static int
HGE_trigger_generic(void* t, const void* data)
{
  return HGE_TRUE;
}

/* trigger add functions */
static void*
HGE_trigger_add_mouse(int type, va_list ap)
{
  HGE_trigger_mouse_t* tgr;

  if((tgr = HGE_alloc(HGE_trigger_mouse_t)) != NULL)
    {
      tgr->box     = *(SDL_Rect*)va_arg(ap, SDL_Rect*);
      tgr->buttons = (Uint8)va_arg(ap, unsigned int);
      tgr->box.w   += tgr->box.x;
      tgr->box.h   += tgr->box.y;

      switch(type)
	{
	case HGE_MOUSE_FOCUS:
	  tgr->handler = HGE_trigger_mouse_focus;
	  break;
	case HGE_MOUSE_UNFOCUS:
	  tgr->handler = HGE_trigger_mouse_unfocus;
	  break;
	}

      SDL_EV_TGRS[SDL_MOUSEMOTION] = 
	HGE_slist_prepend(SDL_EV_TGRS[SDL_MOUSEMOTION], tgr);
    }

  return tgr;
}

static void*
HGE_trigger_add_active(int type, va_list ap)
{
  HGE_trigger_active_t* tgr;

  if((tgr = HGE_alloc(HGE_trigger_active_t)) != NULL)
    {
      tgr->gain  = (Uint8)va_arg(ap, unsigned int);
      tgr->state = (Uint8)va_arg(ap, unsigned int);
      tgr->handler = HGE_trigger_active;
      
      SDL_EV_TGRS[SDL_ACTIVEEVENT] =
	HGE_slist_prepend(SDL_EV_TGRS[SDL_ACTIVEEVENT], tgr);
    }
  
  return tgr;
}
  
static void*
HGE_trigger_add_generic(int type)
{
  HGE_trigger_t* tgr;

  if((tgr = HGE_alloc(HGE_trigger_t)) != NULL)
    {
      tgr->handler = HGE_trigger_generic;

      SDL_EV_TGRS[type] =
	HGE_slist_prepend(SDL_EV_TGRS[type], tgr);
    }

  return tgr;
}

/* global functions */
void
HGE_trigger_init_system(void)
{
  int i;
  for(i = 0; i < SDL_NUMEVENTS; i++)
    SDL_EV_TGRS[i] = NULL;
  SDL_SetEventFilter(HGE_trigger_event_filter);
}

void
HGE_trigger_clean_system(void)
{
  int i;

  for(i = 0; i < SDL_NUMEVENTS; i++)
    {
      HGE_slist_foreach0(SDL_EV_TGRS[i], (HGE_func0_t)HGE_free);
      HGE_slist_free(SDL_EV_TGRS[i]);
    }
}

void*
HGE_trigger_add(int type, HGE_message_t* msg, int count, ...)
{
  va_list ap;
  HGE_trigger_t* tgr;

  va_start(ap, count);
  switch(type & 0xFF00)
    {
    case HGE_MOUSE_TGR:
      tgr = HGE_trigger_add_mouse(type, ap);
      break;
    case HGE_QUIT_TGR:
      tgr = HGE_trigger_add_generic(SDL_QUIT);
      break;
    case HGE_ACTIVE_TGR:
      tgr = HGE_trigger_add_active(type, ap);
      break;
    default:
      tgr = NULL;
      break;
    }
  va_end(ap);

  if(tgr != NULL)
    {
      tgr->count  = count;
      tgr->active = HGE_TRUE;
      tgr->msg    = msg;
    }

  return tgr;
}


int
HGE_trigger_message_handler(HGE_message_t* msg)
{
  HGE_trigger_t* trigger = msg->receiver;

  if(trigger != NULL)
    {
      switch(msg->id)
	{
	case HGE_TRIGGER_ENABLE:
	  trigger->active = HGE_TRUE;
	  break;
	case HGE_TRIGGER_DISABLE:
	  trigger->active = HGE_FALSE;
	  break;
	default:
	  break;
	}
    }

  return HGE_FALSE;
}

/* static functions */

