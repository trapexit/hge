#ifndef _HGE_MAILBOX_H_
#define _HGE_MAILBOX_H_

#include "HGE_types.h"
#include "HGE_queue.h"
#include "HGE_message.h"

typedef struct HGE_mailbox_t HGE_mailbox_t;
struct HGE_mailbox_t
{
  HGE_queue_t*          inbox;
  HGE_message_filter_t  filter;
  HGE_message_handler_t handler;
  Uint32                suspended;
};

extern HGE_typeinfo_t HGE_mailbox[];

HGE_error_t HGE_mailbox_init(void* ptr);
void        HGE_mailbox_clean(void* ptr);
void*       HGE_mailbox_alloc(void);
void        HGE_mailbox_free(void* ptr);

#endif /* _HGE_MAILBOX_H_ */
