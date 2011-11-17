#ifndef _HGE_TYPEINFO_H_
#define _HGE_TYPEINFO_H_

#include "HGE_error.h"

typedef struct HGE_typeinfo_t HGE_typeinfo_t;
struct HGE_typeinfo_t
{
  unsigned int size;
  HGE_error_t (*init)(void*);
  void (*clean)(void*);
  void* msg_filter;
  void* msg_handler;
};

#endif /* _HGE_TYPEINFO_H_ */
