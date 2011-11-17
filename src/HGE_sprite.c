#include <stdlib.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_time.h"
#include "HGE_parray.h"
#include "HGE_slist.h"
#include "HGE_list.h"
#include "HGE_image.h"
#include "HGE_misc.h"
#include "HGE_sprite.h"
#include "HGE_message.h"

HGE_typeinfo_t HGE_sprite[] = {{sizeof(HGE_sprite_t),
				HGE_sprite_init,
				HGE_sprite_clean,
				HGE_sprite_message_filter,
				HGE_sprite_message_handler}};

/* global functions */
HGE_error_t
HGE_sprite_init(void* ptr)
{
  HGE_sprite_t* s = ptr;

  s->x = 0;
  s->y = 0;
  s->flags = 0;
  s->image = NULL;
  s->data  = NULL;
  HGE_transform_init(&s->transform);

  return HGE_SUCCESS;
}

void
HGE_sprite_clean(void* ptr)
{
  HGE_sprite_t* spr;

  if((spr = ptr) != NULL)
    {
      HGE_free_by_type(spr->image, spr->flags);
    }
}

void*
HGE_sprite_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_sprite_t)) != NULL)
    HGE_sprite_init(ptr);

  return ptr;
}

void
HGE_sprite_free(void* ptr)
{
  HGE_sprite_clean(ptr);
  HGE_free(ptr);
}

void*
HGE_sprite_alloc_from_file(const char* path, SDL_Rect* rect)
{
  HGE_sprite_t* spr;

  if((spr = HGE_sprite_alloc()) != NULL)
    {
      if((spr->image = HGE_image_alloc_from_file(path, rect)) == NULL)
	{
	  HGE_sprite_free(spr);
	  return NULL;
	}
      spr->flags |= HGE_FREE;
    }

  return spr;
}

void*
HGE_sprite_alloc_from_surface(SDL_Surface* surface, SDL_Rect* rect)
{
  HGE_sprite_t* spr;

  if((spr = HGE_sprite_alloc()) != NULL)
    {
      if((spr->image = HGE_image_alloc_from_surface(surface, rect)) == NULL)
	{
	  HGE_sprite_free(spr);
	  return NULL;
	}
      spr->flags |= HGE_FREE;
    }

  return spr;
}

void*
HGE_sprite_new_from_file(const char* path, SDL_Rect* rect)
{
  HGE_sprite_t* spr;

  if((spr = HGE_new(HGE_sprite)) != NULL)
    {
      if((spr->image = HGE_image_new_from_file(path, rect)) == NULL)
	{
	  HGE_delete(spr);
	  return NULL;
	}
      spr->flags |= HGE_UNREF;
    }

  return spr;
}

void*
HGE_sprite_new_from_surface(SDL_Surface* surface, SDL_Rect* rect)
{
  HGE_sprite_t* spr;

  if((spr = HGE_new(HGE_sprite)) != NULL)
    {
      if((spr->image = HGE_image_new_from_surface(surface, rect)) == NULL)
	{
	  HGE_delete(spr);
	  return NULL;
	}
      spr->flags |= HGE_UNREF;
    }

  return spr;
}

int
HGE_sprite_make_from_file(HGE_sprite_t* s, const char* path, SDL_Rect* rect)
{
  
  return HGE_SUCCEEDED;
}

int
HGE_sprite_make_from_surface(HGE_sprite_t* s, SDL_Surface* surface, SDL_Rect* rect)
{
  return HGE_SUCCEEDED;
}

void
HGE_sprite_draw(HGE_sprite_t* s)
{
  HGE_image_draw_transformed(s->image, s->x, s->y, &s->transform);
}

void
HGE_sprite_drawv(HGE_sprite_t* s, unsigned int n)
{
  while(n--)
    {
      HGE_image_draw_transformed(s->image, s->x, s->y, &s->transform);
      s++;
    }
}

void
HGE_sprite_drawpv(HGE_sprite_t** s, unsigned int n)
{
  while(n--)
    {
      HGE_image_draw_transformed((*s)->image, (*s)->x, (*s)->y, &(*s)->transform);
      s++;
    }
}

void
HGE_sprite_draw_parray(HGE_parray_t* a)
{
  HGE_sprite_drawpv((HGE_sprite_t**)a->ptrs, a->length);
}

void
HGE_sprite_draw_slist(HGE_slist_t* sl)
{
  while(sl != NULL)
    {
      HGE_sprite_draw(sl->data);
      sl = sl->next;
    }
}

void
HGE_sprite_draw_list(HGE_list_t* l)
{
  while(l != NULL)
    {
      HGE_sprite_draw(l->data);
      l = l->next;
    }
}

int
HGE_sprite_message_filter(HGE_queue_t* inbox, HGE_message_t* msg)
{
  HGE_list_t* node;
  if((msg->id & 0x0100) == 0x0100)
    {
      node = inbox->head;
      while(node != NULL)
	{
	  if(msg->id == ((HGE_message_t*)node->data)->id)
	    {
	      HGE_unref((HGE_message_t*)node->data);
	      node->data = msg;
	      break;
	    }
	  node = node->next;
	}
    }
  
  return HGE_MSG_ALLOW;
}

int
HGE_sprite_message_handler(HGE_message_t* msg)
{
  HGE_arg_t* args;
  HGE_sprite_t*  spr;

  spr = msg->receiver;
  if(spr == NULL)
    return HGE_MSG_REMOVE;

  args = msg->argv->args;
  switch(msg->id)
    {
    case HGE_SPR_SET_X:
      spr->x                  = args[0].u.f;
      break;
    case HGE_SPR_SET_Y:
      spr->y                  = args[0].u.f;
      break;
    case HGE_SPR_SET_R:
      spr->transform.rotation = args[0].u.f;
      break;
    case HGE_SPR_SET_S:
      spr->transform.scale    = args[0].u.f;
      break;
    case HGE_SPR_SET_F:
      spr->transform.flags    = args[0].u.u32;
      break;
    case HGE_SPR_SET_XY:
      spr->x                  = args[0].u.f;
      spr->y                  = args[1].u.f;
      break;
    case HGE_SPR_SET_RS:
      spr->transform.rotation = args[0].u.f;
      spr->transform.scale    = args[1].u.f;
      break;
    case HGE_SPR_SET_XYR:
      spr->x                  = args[0].u.f;
      spr->y                  = args[1].u.f;
      spr->transform.rotation = args[2].u.f;
      break;
    case HGE_SPR_SET_XYS:
      spr->x                  = args[0].u.f;
      spr->y                  = args[1].u.f;
      spr->transform.scale    = args[2].u.f;      
      break;
    case HGE_SPR_SET_XYRS:
      spr->x                  = args[0].u.f;
      spr->y                  = args[1].u.f;
      spr->transform.rotation = args[2].u.f;
      spr->transform.scale    = args[3].u.f;
      break;
    case HGE_SPR_SET_IMG:
      spr->image              = args[0].u.p;
      break;
    case HGE_SPR_INC_X:
      spr->x                  += args[0].u.f;
      break;
    case HGE_SPR_INC_Y:
      spr->y                  += args[0].u.f;
      break;
    case HGE_SPR_INC_R:
      spr->transform.rotation += args[0].u.f;
      break;
    case HGE_SPR_INC_S:
      spr->transform.scale    += args[0].u.f;
      break;
    case HGE_SPR_INC_XY:
      spr->x                  += args[0].u.s32;
      spr->y                  += args[1].u.s32;
      break;
    case HGE_SPR_INC_RS:
      spr->transform.rotation += args[0].u.f;
      spr->transform.scale    += args[1].u.f;
      break;
    case HGE_SPR_INC_XYR:
      spr->x                  += args[0].u.f;
      spr->y                  += args[1].u.f;
      spr->transform.rotation += args[2].u.f;
      break;
    case HGE_SPR_INC_XYS:
      spr->x                  += args[0].u.f;
      spr->y                  += args[1].u.f;
      spr->transform.scale    += args[2].u.f;
      break;
    case HGE_SPR_INC_XYRS:
      spr->x                  += args[0].u.f;
      spr->y                  += args[1].u.f;
      spr->transform.rotation += args[2].u.f;
      spr->transform.scale    += args[3].u.f;
      break;
    case HGE_SPR_CINC_X:
      if(args[0].u.s32 > 0)
	{
	  if(spr->x < args[1].u.f)
	    {
	      spr->x += args[0].u.f;
	      return HGE_MSG_KEEP;
	    }
	}
      else
	{
	  if(spr->x > args[1].u.f)
	    {
	      spr->x += args[0].u.f;
	      return HGE_MSG_KEEP;
	    }
	}
      break;
    case HGE_SPR_CINC_Y:
      if(args[0].u.s32 > 0)
	{
	  if(spr->y < args[1].u.f)
	    {
	      spr->y += args[0].u.f;
	      return HGE_MSG_KEEP;
	    }
	}
      else
	{
	  if(spr->y > args[1].u.f)
	    {
	      spr->y += args[0].u.f;
	      return HGE_MSG_KEEP;
	    }
	}
      break;
    case HGE_SPR_GOTO_XY:
      args[0].u.fa[1] = spr->x;
      args[1].u.fa[1] = spr->y;
      args[2].u.fa[1] = ((float)msg->dispatch_time / 1000.0);
      msg->id = HGE_SPR_GOTO_XY2;

    case HGE_SPR_GOTO_XY2:
      {
	float alpha = ((HGE_time_getf() - args[2].u.fa[1]) / args[2].u.f);
	if(alpha <= 1.0)
	  {
	    spr->x = (((1.0-alpha)*args[0].u.fa[1])+(alpha*args[0].u.f));
	    spr->y = (((1.0-alpha)*args[1].u.fa[1])+(alpha*args[1].u.f));
	    return HGE_MSG_KEEP;
	  }
      }
      break;
    default:
      break;
    }

  return HGE_MSG_REMOVE;
}

