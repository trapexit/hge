#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "SDL.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_mem.h"

#define SIZE_TO_ALLOC(SIZE) (offsetof(rc_info, data)+(SIZE))
#define GET_RC_INFO(PTR)    ((rc_info*)((char*)ptr-offsetof(rc_info,data)))

typedef struct rc_info rc_info;
struct rc_info
{
  unsigned int    count;
  unsigned int    ref;
  HGE_typeinfo_t* type;
  union
  {
    long   l;
    double d;
    void*  p;
  }data[1];
};

void*
_HGE_cmalloc(size_t ds)
{
  return calloc(1, ds);
}

void* (*HGE_malloc)(size_t)         = malloc;
void* (*HGE_cmalloc)(size_t)        = _HGE_cmalloc;
void* (*HGE_remalloc)(void*,size_t) = realloc;
void  (*HGE_free)(void*)            = free;

/* ref count functions */
/* private */
enum
{
  DONT_INIT = 0,
  INIT      = !DONT_INIT
};
                                    
void*
hge_new(HGE_typeinfo_t* type, size_t count, int init)
{
  rc_info* rc;

  if((rc=HGE_malloc(SIZE_TO_ALLOC(type->size*count))) != NULL)
    {
      rc->type  = type;
      rc->count = count;
      rc->ref   = 1;
      if(init)
        {
          char* itr = (char*)rc->data;
          while(count--)
            {
              type->init(itr);
              itr += type->size;
            }
        }

      return rc->data;
    }
  
  return NULL;
}

/* public */                                    
void*
HGE_new(HGE_typeinfo_t* type)
{
  return hge_new(type, 1, INIT);
}

void*
HGE_newU(HGE_typeinfo_t* type)
{
  return hge_new(type, 1, DONT_INIT);
}

void*
HGE_newV(HGE_typeinfo_t* type, size_t c)
{
  return hge_new(type, c, INIT);
}

void*
HGE_newVU(HGE_typeinfo_t* type, size_t c)
{
  return hge_new(type, c, DONT_INIT);
}

void
HGE_delete(void* ptr)
{
  rc_info* rc;
  char*    itr;

  if(ptr != NULL)
    {
      rc = GET_RC_INFO(ptr);
      itr = (char*)rc->data;
      while(rc->count--)
	{
	  rc->type->clean(itr);
	  itr += rc->type->size;
	}
      HGE_free(rc);
    }
}

void*
HGE_ref(void* ptr)
{
  if(ptr != NULL)
    GET_RC_INFO(ptr)->ref++;

  return ptr;
}

void*
HGE_unref(void* ptr)
{
  rc_info* rc;
  char*    itr;

  if(ptr != NULL)
    {
      rc = GET_RC_INFO(ptr);
      if(!(--rc->ref))
	{
	  itr = (char*)rc->data;
	  while(rc->count--)
	    {
	      rc->type->clean(itr);
	      itr += rc->type->size;
	    }

	  HGE_free(rc);
	  return NULL;
	}
    }

  return ptr;
}

unsigned int
HGE_refcount(void* ptr)
{
  return ((ptr != NULL) ? GET_RC_INFO(ptr)->ref : 0);
}

void
HGE_free_by_type(void* ptr, int type)
{
  switch(type & HGE_FREE_TYPE_MASK)
    {
    case HGE_FREE:
      HGE_free(ptr);
      break;
    case HGE_UNREF:
      HGE_unref(ptr);
      break;
    case HGE_DELETE:
      HGE_delete(ptr);
      break;
    default:
      break;
    }
}

void*
HGE_make_rc(HGE_typeinfo_t* type, void* ds)
{
  rc_info* rc;

  if((rc=HGE_remalloc(ds, type->size)) != NULL)
    {
      rc->type  = type;
      rc->count = 1;
      rc->ref   = 1;
      memmove(rc->data, ds, type->size);

      return rc->data;
    }

  return NULL;
}
