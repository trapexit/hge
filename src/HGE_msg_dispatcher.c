#include "HGE_mem.h"
#include "HGE_msg_dispatcher.h"
#include "HGE_mailbox.h"

/* Private functions */
static inline HGE_mailbox_t*
hge_postoffice_mailbox_lookup(HGE_hash_t* po, void* ptr)
{
  HGE_mailbox_t* mailbox;

  mailbox = HGE_hash_lookup(po, ptr);
  if(mailbox == NULL)
    {
      mailbox = HGE_alloc(HGE_mailbox_t);
      mailbox->suspended = 0;
      mailbox->inbox     = HGE_queue_alloc();
      mailbox->filter    = NULL;
      mailbox->handler   = NULL;

      HGE_hash_insert(po, ptr, mailbox);
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
HGE_error_t
HGE_msg_dispatcher_init(void* ptr)
{
  HGE_msg_dispatcher_t* md = ptr;

  md->msgs            = NULL;
  md->default_filter  = NULL;
  md->default_handler = NULL;
  md->postoffice      = HGE_hash_alloc();
  HGE_hash_set_value_free(md->postoffice, hge_postoffice_value_free);

  return ((md->postoffice == NULL) ?
          HGE_HASH_ALLOC_FAILED : HGE_SUCCESS);
}

void
HGE_msg_dispatcher_clean(void* ptr)
{
  HGE_msg_dispatcher_t* md = ptr;

  HGE_slist_free(md->msgs);
  HGE_hash_free(md->postoffice);
}

void*
HGE_msg_dispatcher_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_msg_dispatcher_t)) != NULL)
    {
      if(HGE_msg_dispatcher_init(ptr) != HGE_SUCCESS)
        {
          HGE_free_and_null(ptr);
        }
    }

  return ptr;
}

void
HGE_msg_dispatcher_free(void* ptr)
{
  if(ptr != NULL)
    {
      HGE_msg_dispatcher_clean(ptr);
      HGE_free(ptr);
    }
}

void
HGE_msg_dispatcher_dispatch(HGE_msg_dispatcher_t* md, HGE_message_t* msg)
{
  HGE_mailbox_t* mb = hge_postoffice_mailbox_lookup(md->postoffice, msg->receiver);

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
HGE_msg_dispatcher_process(HGE_msg_dispatcher_t* md)
{
  HGE_slist_t* node;
  HGE_slist_t* next;
  HGE_message_t* msg;

  node = md->msgs;
  while(node != NULL)
    {
      msg  = node->data;
      next = node->next;
      if((SDL_GetTicks() >= msg->dispatch_time))
        {
          HGE_msg_dispatcher_dispatch(md, msg);
          md->msgs = HGE_slist_delete_link(md->msgs, node);
        }
      else
        break;
      node = next;
    }

  HGE_hash_foreach0(md->postoffice, hge_postoffice_foreach0);
}

void
HGE_msg_dispatcher_send(HGE_msg_dispatcher_t* md,
                        Uint32 when,
                        void* sender, void* receiver,
                        Uint32 id, HGE_argv_t* argv)
{
  HGE_message_t* msg;

  msg = HGE_newU(HGE_message);
  msg->sender = sender;
  msg->receiver = receiver;
  msg->dispatch_time = when;
  msg->id = id;
  msg->argv = argv;

  md->msgs = HGE_slist_insert_sorted(md->msgs, hge_postoffice_time_cmp, msg);
}

void
HGE_msg_dispatcher_send_msg(HGE_msg_dispatcher_t* md, Uint32 when, HGE_message_t* msg)
{
  msg->dispatch_time = when;

  md->msgs = HGE_slist_insert_sorted(md->msgs, hge_postoffice_time_cmp, msg);
}

void
HGE_msg_dispatcher_purge(HGE_msg_dispatcher_t* md, void* ptr)
{
  HGE_hash_remove(md->postoffice, ptr);
}

HGE_queue_t*
HGE_msg_dispatcher_steal(HGE_msg_dispatcher_t* md, void* ptr)
{
  HGE_queue_t* inbox;
  HGE_mailbox_t* mb = hge_postoffice_mailbox_lookup(md->postoffice, ptr);

  inbox = mb->inbox;
  mb->inbox = HGE_queue_alloc();

  return inbox;
}

HGE_queue_t*
HGE_msg_dispatcher_copy(HGE_msg_dispatcher_t* md, void* ptr)
{
  HGE_queue_t* inbox;

  inbox = HGE_queue_copy(hge_postoffice_mailbox_lookup(md->postoffice, ptr)->inbox);
  HGE_queue_foreach0(inbox, (HGE_func0_t)HGE_ref);

  return inbox;
}
