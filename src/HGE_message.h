#ifndef _HGE_MESSAGE_H_
#define _HGE_MESSAGE_H_

#include "SDL.h"
#include "HGE_queue.h"
#include "HGE_argv.h"

enum
  {
    HGE_MSG_REJECT = 0,
    HGE_MSG_ALLOW  = 1,
    HGE_MSG_REMOVE = 0,
    HGE_MSG_KEEP   = 1
  };

typedef struct HGE_message_t HGE_message_t;
struct HGE_message_t
{
  Uint32      dispatch_time;
  void*       sender;
  void*       receiver;
  Uint32      id;
  HGE_argv_t* argv;
};

typedef int (*HGE_message_filter_t)(HGE_queue_t*,HGE_message_t*);
typedef int (*HGE_message_handler_t)(HGE_message_t*);

extern HGE_typeinfo_t HGE_message[];

HGE_error_t HGE_message_init(void* ptr);
void        HGE_message_clean(void* ptr);
void*       HGE_message_alloc(void);
void        HGE_message_free(void* ptr);

#endif /* _HGE_MESSAGE_H_ */
