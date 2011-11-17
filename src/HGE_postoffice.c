#include "SDL.h"
#include "HGE_defines.h"
#include "HGE_mem.h"
#include "HGE_slist.h"
#include "HGE_list.h"
#include "HGE_hash.h"
#include "HGE_message.h"
#include "HGE_mailbox.h"
#include "HGE_postoffice.h"


/* static local variables */
static SDL_mutex* _msgs_mutex_;
static SDL_mutex* _postoffice_mutex_;
static HGE_slist_t* _msgs_;
static HGE_hash_t*  _postoffice_;
static HGE_message_filter_t  _default_filter_  = HGE_postoffice_default_filter;
static HGE_message_handler_t _default_handler_ = HGE_postoffice_default_handler;

/* static local functions */
static HGE_mailbox_t* hge_postoffice_mailbox_lookup(void*);
static int            hge_postoffice_time_cmp(const void*, const void*);
static void           hge_postoffice_value_free(void*);
static void           hge_postoffice_foreach0(void*, void*);

static HGE_mailbox_t*
hge_postoffice_mailbox_lookup(void* ptr)
{
  HGE_mailbox_t* mailbox;

  SDL_LockMutex(_postoffice_mutex_);
  mailbox = HGE_hash_lookup(_postoffice_, ptr);
  SDL_UnlockMutex(_postoffice_mutex_);
  if(mailbox == NULL)
    {
      mailbox = HGE_alloc(HGE_mailbox_t);
      
      mailbox->suspended = 0;
      mailbox->inbox     = HGE_queue_alloc();
      mailbox->filter    = _default_filter_;
      mailbox->handler   = _default_handler_;

      SDL_LockMutex(_postoffice_mutex_);
      HGE_hash_insert(_postoffice_, ptr, mailbox);
      SDL_UnlockMutex(_postoffice_mutex_);
    }

  return mailbox;
}

static int
hge_postoffice_time_cmp(const void* m0, const void* m1)
{
  return (int)(((HGE_message_t*)m0)->dispatch_time - ((HGE_message_t*)m1)->dispatch_time);
}

static void
hge_postoffice_value_free(void* value)
{
  HGE_mailbox_free(value);
}

static void
hge_postoffice_foreach0(void* key, void* value)
{
  HGE_list_t* node;
  HGE_list_t* next;
  HGE_mailbox_t* mb = value;

  if(!mb->suspended)
    {
      node = mb->inbox->head;
      while(node != NULL)
	{
	  next = node->next;
	  
	  if(mb->handler((HGE_message_t*)node->data) == HGE_MSG_REMOVE)
	    {
	      HGE_unref((HGE_message_t*)node->data);
	      HGE_queue_delete_link(mb->inbox, node);
	    }
      
	  node = next;
	}
    }
}

/* global functions */
void
HGE_postoffice_init_system(void)
{
  _msgs_ = NULL;
  _postoffice_ = HGE_hash_alloc();
  HGE_hash_set_value_free(_postoffice_, hge_postoffice_value_free);
  _msgs_mutex_       = SDL_CreateMutex();
  _postoffice_mutex_ = SDL_CreateMutex();
}

void
HGE_postoffice_clean_system(void)
{
  HGE_slist_foreach0(_msgs_, (HGE_func0_t)HGE_delete);
  HGE_slist_free(_msgs_);
  HGE_hash_free(_postoffice_);
  SDL_DestroyMutex(_postoffice_mutex_);
  SDL_DestroyMutex(_msgs_mutex_);
}

int
HGE_postoffice_default_filter(HGE_queue_t* inbox, HGE_message_t* msg)
{
  return HGE_MSG_ALLOW;
}

int
HGE_postoffice_default_handler(HGE_message_t* msg)
{
  return HGE_MSG_REMOVE;
}

void
HGE_postoffice_set_fh(void* ptr, HGE_typeinfo_t* type)
{
  HGE_mailbox_t* mb = hge_postoffice_mailbox_lookup(ptr);

  if(type->msg_filter != NULL)
    mb->filter  = type->msg_filter;
  if(type->msg_handler != NULL)
    mb->handler = type->msg_handler;
}

void
HGE_postoffice_set_filter(void* ptr, HGE_message_filter_t filter)
{
  hge_postoffice_mailbox_lookup(ptr)->filter = filter;
}

void
HGE_postoffice_set_handler(void* ptr, HGE_message_handler_t handler)
{
  hge_postoffice_mailbox_lookup(ptr)->handler = handler;
}

void
HGE_postoffice_set_default_filter(HGE_message_filter_t filter)
{
  _default_filter_ = filter;
}

void
HGE_postoffice_set_default_handler(HGE_message_handler_t handler)
{
  _default_handler_ = handler;
}

HGE_message_filter_t
HGE_postoffice_get_filter(void* ptr)
{
  return hge_postoffice_mailbox_lookup(ptr)->filter;
}

HGE_message_handler_t
HGE_postoffice_get_handler(void* ptr)
{
  return hge_postoffice_mailbox_lookup(ptr)->handler;
}

HGE_message_filter_t
HGE_postoffice_get_default_filter(void)
{
  return _default_filter_;
}

HGE_message_handler_t
HGE_postoffice_get_default_handler(void)
{
  return _default_handler_;
}

void
HGE_postoffice_send(Uint32 when, void* sender, void* receiver, Uint32 id, HGE_argv_t* argv)
{
  HGE_message_t* msg;

  msg                = HGE_newU(HGE_message);
  msg->sender        = sender;
  msg->receiver      = receiver;
  msg->dispatch_time = when;
  msg->id            = id;
  msg->argv          = argv;

  SDL_LockMutex(_msgs_mutex_);
  _msgs_ = HGE_slist_insert_sorted(_msgs_, hge_postoffice_time_cmp, msg);
  SDL_UnlockMutex(_msgs_mutex_);
}

void
HGE_postoffice_send_msg(Uint32 when, HGE_message_t* msg)
{
  msg->dispatch_time = when;

  SDL_LockMutex(_msgs_mutex_);
  _msgs_ = HGE_slist_insert_sorted(_msgs_, hge_postoffice_time_cmp, msg);
  SDL_UnlockMutex(_msgs_mutex_);
}

void
HGE_postoffice_dispatch(HGE_message_t* msg)
{
  HGE_mailbox_t* mb = hge_postoffice_mailbox_lookup(msg->receiver);

  switch(mb->filter(mb->inbox, msg))
    {
    default:
    case HGE_MSG_ALLOW:
      HGE_queue_push_head(mb->inbox, msg);
      break;
    case HGE_MSG_REJECT:
      HGE_unref(msg);
      break;
    }
}

void
HGE_postoffice_process(void)
{
  HGE_slist_t* node;
  HGE_slist_t* next;
  HGE_message_t* msg;

  node = _msgs_;
  while(node != NULL)
    {
      msg  = node->data;
      next = node->next;
      if((SDL_GetTicks() >= msg->dispatch_time))
	{
	  HGE_postoffice_dispatch(msg);
	  SDL_LockMutex(_msgs_mutex_);
	  _msgs_ = HGE_slist_delete_link(_msgs_, node);
	  SDL_UnlockMutex(_msgs_mutex_);
	}
      else
	break;
      node = next;
    }

  HGE_hash_foreach0(_postoffice_, hge_postoffice_foreach0);
}

HGE_queue_t*
HGE_postoffice_steal(void* ptr)
{
  HGE_queue_t* inbox;
  HGE_mailbox_t* mb = hge_postoffice_mailbox_lookup(ptr);

  inbox = mb->inbox;
  mb->inbox = HGE_queue_alloc();

  return inbox;
}

HGE_queue_t*
HGE_postoffice_copy(void* ptr)
{
  HGE_queue_t* inbox;

  inbox = HGE_queue_copy(hge_postoffice_mailbox_lookup(ptr)->inbox);
  HGE_queue_foreach0(inbox, (HGE_func0_t)HGE_ref);
  
  return inbox;
}

Uint32
HGE_postoffice_suspend(void* ptr)
{
  Uint32 prev;
  HGE_mailbox_t* mb = hge_postoffice_mailbox_lookup(ptr);

  prev = mb->suspended;
  mb->suspended = SDL_GetTicks();

  return prev;
}

Uint32
HGE_postoffice_resume(void* ptr)
{
  Uint32 prev;
  HGE_mailbox_t* mb = hge_postoffice_mailbox_lookup(ptr);

  prev = mb->suspended;
  mb->suspended = 0;

  return prev;
}

void
HGE_postoffice_purge(void* ptr)
{
  SDL_LockMutex(_postoffice_mutex_);
  HGE_hash_remove(_postoffice_, ptr);
  SDL_UnlockMutex(_postoffice_mutex_);
}
