#ifndef _HGE_SLIST_H_
#define _HGE_SLIST_H_

#include "HGE_types.h"

#define HGE_slist_next(slist) ((slist) ? ((slist)->next) : NULL)

typedef struct HGE_slist_t HGE_slist_t;
struct HGE_slist_t
{
  void* data;
  HGE_slist_t* next;
};

HGE_slist_t* HGE_slist_alloc(void);
void         HGE_slist_init(HGE_slist_t* slist);
void         HGE_slist_free(HGE_slist_t* slist);
void         HGE_slist_free_link(HGE_slist_t* slist);
HGE_slist_t* HGE_slist_append(HGE_slist_t* slist, void* data);
HGE_slist_t* HGE_slist_prepend(HGE_slist_t* slist, void* data);
HGE_slist_t* HGE_slist_insert(HGE_slist_t* slist, void* data, unsigned int pos);
HGE_slist_t* HGE_slist_insert_before(HGE_slist_t* slist, HGE_slist_t* sib, void* data);
HGE_slist_t* HGE_slist_concat(HGE_slist_t* slist0, HGE_slist_t* slist1);
HGE_slist_t* HGE_slist_remove(HGE_slist_t* slist, const void* data);
HGE_slist_t* HGE_slist_remove_all(HGE_slist_t* slist, const void* data);
HGE_slist_t* HGE_slist_remove_link(HGE_slist_t* slist, HGE_slist_t* link);
HGE_slist_t* HGE_slist_delete_link(HGE_slist_t* slist, HGE_slist_t* link);
HGE_slist_t* HGE_slist_copy(HGE_slist_t* slist);
HGE_slist_t* HGE_slist_reverse(HGE_slist_t* slist);
HGE_slist_t* HGE_slist_nth(HGE_slist_t* slist, unsigned int n);
void*        HGE_slist_nth_data(HGE_slist_t* slist, unsigned int n);
HGE_slist_t* HGE_slist_find(HGE_slist_t* slist, void* data);
int          HGE_slist_position(HGE_slist_t* slist, HGE_slist_t* link);
int          HGE_slist_index(HGE_slist_t* slist, const void* data);
HGE_slist_t* HGE_slist_last(HGE_slist_t* slist);
int          HGE_slist_length(HGE_slist_t* slist);
void         HGE_slist_foreach0(HGE_slist_t* slist, HGE_func0_t func);
void         HGE_slist_foreach1(HGE_slist_t* slist, HGE_func1_t func, void* data);
HGE_slist_t* HGE_slist_insert_sorted(HGE_slist_t* slist, HGE_cfunc_t func, void* data);

				     
#endif
