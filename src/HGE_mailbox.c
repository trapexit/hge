#include "SDL.h"
#include "HGE_defines.h"
#include "HGE_typeinfo.h"
#include "HGE_mem.h"
#include "HGE_mailbox.h"

HGE_typeinfo_t HGE_mailbox[] = {{sizeof(HGE_mailbox_t),
				 HGE_mailbox_init,
				 HGE_mailbox_clean,
				 NULL,
				 NULL}};

HGE_error_t
HGE_mailbox_init(void* ptr)
{
  HGE_mailbox_t* mb = ptr;

  mb->inbox     = NULL;
  mb->filter    = NULL;
  mb->handler   = NULL;
  mb->suspended = 0;

  return HGE_SUCCESS;
}

void
HGE_mailbox_clean(void* ptr)
{
  HGE_mailbox_t* mb = ptr;

  if(mb != NULL)
    {
      HGE_queue_foreach0(mb->inbox, (HGE_func0_t)HGE_unref);
      HGE_queue_free(mb->inbox);
    }
}

void*
HGE_mailbox_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_mailbox_t)) != NULL)
    HGE_mailbox_init(ptr);

  return ptr;
}

void
HGE_mailbox_free(void* ptr)
{
  HGE_mailbox_clean(ptr);
  HGE_free(ptr);
}
