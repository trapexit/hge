#include "SDL.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_argv.h"
#include "HGE_message.h"

HGE_typeinfo_t HGE_message[] = {{sizeof(HGE_message_t),
				 HGE_message_init,
				 HGE_message_clean,
				 NULL,
				 NULL}};

/* global functions */
HGE_error_t
HGE_message_init(void* ptr)
{
  HGE_message_t* msg = ptr;

  msg->dispatch_time = 0;
  msg->sender        = NULL;
  msg->receiver      = NULL;
  msg->id            = 0;
  msg->argv          = NULL;

  return HGE_SUCCESS;
}

void
HGE_message_clean(void* ptr)
{
  HGE_message_t* msg = ptr;

  if(ptr != NULL)
    {
      HGE_argv_clean(msg->argv);
      HGE_free(msg->argv);
    }
}

void*
HGE_message_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_message_t)) != NULL)
    HGE_message_init(ptr);

  return ptr;
}

void
HGE_message_free(void* ptr)
{
  HGE_message_clean(ptr);
  HGE_free(ptr);
}
