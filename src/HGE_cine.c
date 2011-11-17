#include "SDL.h"
#include "HGE_defines.h"
#include "HGE_gl_funcs.h"
#include "HGE_mem.h"
#include "HGE_time.h"
#include "HGE_postoffice.h"
#include "HGE_sprite.h"
#include "HGE_gl.h"
#include "HGE_cine.h"

HGE_typeinfo_t HGE_cine[] = {{sizeof(HGE_cine_t),
			      HGE_cine_init,
			      HGE_cine_clean,
			      NULL,
			      HGE_cine_message_handler}};


/* static functions */
static void
HGE_cine_dispatch_msgs(HGE_cine_t* cine)
{
  HGE_slist_t* node;
  HGE_slist_t* next;
  HGE_message_t* msg;

  if(!cine->paused)
    {
      node = cine->msgs;
      while(node != NULL)
	{
	  msg = node->data;
	  next = node->next;
	  if((SDL_GetTicks() - cine->start_time) >= msg->dispatch_time)
	    {
	      HGE_postoffice_send_msg(SDL_GetTicks(), msg);
	      cine->msgs = HGE_slist_delete_link(cine->msgs, node);
	    }
	  else
	    break;
	  node = next;
	}
    }
}

static void
HGE_cine_frame_func(HGE_loop_t* loop)
{
  HGE_cine_t* cine = loop->data;

  HGE_clearbuffer();

  HGE_cine_dispatch_msgs(cine);
  HGE_postoffice_process();

  HGE_sprite_draw_slist(cine->sprs);

  HGE_swapbuffers();
}

/* global functions */
HGE_error_t
HGE_cine_init(void* ptr)
{
  HGE_cine_t* cine = ptr;

  cine->start_time = 0;
  cine->paused = HGE_FALSE;
  cine->imgs   = NULL;
  cine->sprs   = NULL;
  cine->msgs   = NULL;
  cine->loop    = HGE_loop_alloc();
  if(cine->loop == NULL)
    {
      return HGE_LOOP_ALLOC_FAILED;
    }

  HGE_loop_set_data(cine->loop, cine);
  HGE_loop_set_framefunc(cine->loop, HGE_cine_frame_func);

  return HGE_SUCCESS;
}

void
HGE_cine_clean(void* ptr)
{
  HGE_cine_t* cine = ptr;

  HGE_slist_foreach0(cine->msgs, (HGE_func0_t)HGE_unref);
  HGE_slist_foreach0(cine->sprs, HGE_postoffice_purge);
  HGE_slist_free(cine->sprs);
  HGE_loop_free(cine->loop);
}

void*
HGE_cine_alloc(void)
{
  void* ptr;

  if((ptr=HGE_alloc(HGE_cine_t)) != NULL)
    {
      if(HGE_cine_init(ptr) != HGE_SUCCESS)
	{
	  HGE_free_and_null(ptr);
	}
    }

  return ptr;
}

void
HGE_cine_free(void* ptr)
{
  HGE_cine_clean(ptr);
  HGE_free(ptr);
}

void
HGE_cine_run(HGE_cine_t* cine)
{
  cine->start_time = HGE_time_geti();
  HGE_loop_run(cine->loop);
}

void
HGE_cine_run_once(HGE_cine_t* cine)
{
  if(!cine->start_time)
    cine->start_time = HGE_time_geti();
  HGE_loop_run_once(cine->loop);
}

void
HGE_cine_add_sprite(HGE_cine_t* cine, HGE_sprite_t* spr)
{
  cine->sprs = HGE_slist_prepend(cine->sprs, spr);
  HGE_postoffice_set_fh(spr, HGE_sprite);
}

void
HGE_cine_pause(HGE_cine_t* cine)
{
  cine->paused = SDL_GetTicks();
  HGE_slist_foreach0(cine->sprs, (HGE_func0_t)HGE_postoffice_suspend);
}

void
HGE_cine_unpause(HGE_cine_t* cine)
{
  cine->paused = 0;
  HGE_slist_foreach0(cine->sprs, (HGE_func0_t)HGE_postoffice_resume);
}

int
HGE_cine_message_handler(HGE_message_t* msg)
{
  HGE_cine_t* cine = msg->receiver;

  switch(msg->id)
    {
    case HGE_CINE_PAUSE:
      HGE_cine_pause(cine);
      break;
    case HGE_CINE_UNPAUSE:
      HGE_cine_unpause(cine);
      break;
    case HGE_CINE_DONE:
      cine->loop->done = HGE_TRUE;
      break;
    default:
      break;
    }

  return HGE_MSG_REMOVE;
}
