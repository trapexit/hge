#ifndef _HGE_MSG_DISPATCHER_H_
#define _HGE_MSG_DISPATCHER_H_

#include "SDL.h"
#include "HGE_types.h"
#include "HGE_slist.h"
#include "HGE_hash.h"
#include "HGE_message.h"

typedef struct HGE_msg_dispatcher_t HGE_msg_dispatcher_t;
struct HGE_msg_dispatcher_t
{
  HGE_slist_t* msgs;
  HGE_hash_t*  postoffice;
  HGE_message_filter_t default_filter;
  HGE_message_handler_t default_handler;
};

HGE_error_t HGE_msg_dispatcher_init(void* ptr);
void        HGE_msg_dispatcher_clean(void* ptr);
void*       HGE_msg_dispatcher_alloc(void);
void        HGE_msg_dispatcher_free(void* ptr);

void HGE_msg_dispatcher_send(HGE_msg_dispatcher_t* md, 
                             Uint32 when,
                             void* sender, void* receiver,
                             Uint32 id, HGE_argv_t* argv);
void HGE_msg_dispatcher_send_msg(HGE_msg_dispatcher_t* md,
                                 Uint32 when, HGE_message_t* msg);
void HGE_msg_dispatcher_process(HGE_msg_dispatcher_t* md);

void         HGE_msg_dispatcher_purge(HGE_msg_dispatcher_t* md, void* ptr);
HGE_queue_t* HGE_msg_dispatcher_steal(HGE_msg_dispatcher_t* md, void* ptr);
HGE_queue_t* HGE_msg_dispatcher_copy(HGE_msg_dispatcher_t* md, void* ptr);


#endif /* _HGE_MSG_DISPATCHER_H_ */
