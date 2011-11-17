#ifndef _HGE_LIST_H_
#define _HGE_LIST_H_

#include "HGE_types.h"

typedef struct HGE_list_t HGE_list_t;
struct HGE_list_t
{
  void*       data;
  HGE_list_t* next;
  HGE_list_t* prev;
};

HGE_list_t*  HGE_list_alloc(void);
void         HGE_list_init(HGE_list_t* list);
void         HGE_list_free(HGE_list_t* list);
void         HGE_list_free_link(HGE_list_t* list);
HGE_list_t*  HGE_list_append(HGE_list_t* list, void* data);
HGE_list_t*  HGE_list_prepend(HGE_list_t* list, void* data);
HGE_list_t*  HGE_list_insert(HGE_list_t* list, void* data, int pos);
HGE_list_t*  HGE_list_insert_before(HGE_list_t* list, HGE_list_t* sib, void* data);
HGE_list_t*  HGE_list_concat(HGE_list_t* list0, HGE_list_t* list1);
HGE_list_t*  HGE_list_remove(HGE_list_t* list, void* data);
HGE_list_t*  HGE_list_remove_link(HGE_list_t* list, HGE_list_t* link);
HGE_list_t*  HGE_list_delete_link(HGE_list_t* list, HGE_list_t* link);
HGE_list_t*  HGE_list_copy(HGE_list_t* list);
HGE_list_t*  HGE_list_reverse(HGE_list_t* list);
HGE_list_t*  HGE_list_nth(HGE_list_t* list, unsigned int n);
HGE_list_t*  HGE_list_nth_prev(HGE_list_t* list, unsigned int n);
void*        HGE_list_nth_data(HGE_list_t* list, unsigned int n);
HGE_list_t*  HGE_list_find(HGE_list_t* list, const void* data);
HGE_list_t*  HGE_list_find_custom(HGE_list_t* list, HGE_cfunc_t func, const void* data);
int          HGE_list_position(HGE_list_t* list, HGE_list_t* link);
int          HGE_list_index(HGE_list_t* list, const void* data);
HGE_list_t*  HGE_list_first(HGE_list_t* list);
HGE_list_t*  HGE_list_last(HGE_list_t* list);
int          HGE_list_length(HGE_list_t* list);
void         HGE_list_foreach0(HGE_list_t* list, HGE_func0_t func);
void         HGE_list_foreach1(HGE_list_t* list, HGE_func1_t func, void* user_data);
HGE_list_t*  HGE_list_insert_sorted(HGE_list_t* list, HGE_cfunc_t func, void* data);


#endif

