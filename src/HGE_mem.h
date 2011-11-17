#ifndef _HGE_MEM_H_
#define _HGE_MEM_H_

#include <stdlib.h>
#include "HGE_typeinfo.h"

enum
  {
    HGE_FREE =   0x01,
    HGE_UNREF =  0x02,
    HGE_DELETE = 0x03,
    HGE_FREE_TYPE_MASK = 0x03
  };

#define HGE_mallocV(S,C)       (HGE_malloc((S)*(C))
#define HGE_cmallocV(S,C)      (HGE_cmalloc((S)*(C)))
#define HGE_alloc(T)           (HGE_malloc(sizeof(T)))
#define HGE_allocV(T,C)        (HGE_malloc(sizeof(T)*(C)))
#define HGE_calloc(T)          (HGE_cmalloc(sizeof(T)))
#define HGE_callocV(T,C)       (HGE_cmalloc(sizeof(T)*(C)))
#define HGE_realloc(P,T,C)     (HGE_remalloc((P),sizeof(T)*(C)))
#define HGE_free_and_null(P)   (HGE_free(P),(P)=NULL)
#define HGE_delete_and_null(P) (HGE_delete(P),(P)=NULL)
#define HGE_unref_and_null(P)  ((HGE_unref(P)==NULL) ? (P)=NULL : (P))

extern void* (*HGE_malloc)(size_t);
extern void* (*HGE_cmalloc)(size_t);
extern void* (*HGE_remalloc)(void*,size_t);
extern void  (*HGE_free)(void*);
void*        HGE_new(HGE_typeinfo_t* t);
void*        HGE_newU(HGE_typeinfo_t* t);
void*        HGE_newV(HGE_typeinfo_t* t, size_t c);
void*        HGE_newVU(HGE_typeinfo_t* t, size_t c);
void         HGE_delete(void* ptr);
void*        HGE_ref(void* ptr);
void*        HGE_unref(void* ptr);
unsigned int HGE_refcount(void* ptr);
void         HGE_free_by_type(void* ptr, int type);
void*        HGE_make_rc(HGE_typeinfo_t* type, void* ds);

#endif /* _HGE_MEM_H_ */

