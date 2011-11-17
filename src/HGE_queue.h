#ifndef _HGE_QUEUE_H_
#define _HGE_QUEUE_H_

#include "HGE_types.h"
#include "HGE_list.h"

typedef struct HGE_queue_t HGE_queue_t;
struct HGE_queue_t
{
  HGE_list_t*  head;
  HGE_list_t*  tail;
  unsigned int length;
};

extern HGE_typeinfo_t HGE_queue[];

HGE_error_t  HGE_queue_init(void* ptr);
void         HGE_queue_clean(void* ptr);
void*        HGE_queue_alloc(void);
void         HGE_queue_free(void* ptr);
int          HGE_queue_is_empty(HGE_queue_t* queue);
unsigned int HGE_queue_get_length(HGE_queue_t* queue);
void         HGE_queue_reverse(HGE_queue_t* queue);
HGE_queue_t* HGE_queue_copy(HGE_queue_t* queue);
void         HGE_queue_foreach0(HGE_queue_t* queue, HGE_func0_t func);
void         HGE_queue_foreach1(HGE_queue_t* queue, HGE_func1_t func, void* user_data);
HGE_list_t*  HGE_queue_find(HGE_queue_t* queue, const void* data);
HGE_list_t*  HGE_queue_find_custom(HGE_queue_t* queue, HGE_cfunc_t func, const void* data);
void         HGE_queue_push_head(HGE_queue_t* queue, void* data);
void         HGE_queue_push_nth(HGE_queue_t* queue, void* data, unsigned int n);
void         HGE_queue_push_head_link(HGE_queue_t* queue, HGE_list_t* link);
void         HGE_queue_push_head_queue(HGE_queue_t* queue0, HGE_queue_t* queue1);
void         HGE_queue_push_tail(HGE_queue_t* queue, void* data);
void         HGE_queue_push_tail_link(HGE_queue_t* queue, HGE_list_t* link);
void         HGE_queue_push_tail_queue(HGE_queue_t* queue0, HGE_queue_t* queue1);
void         HGE_queue_push_nth_link(HGE_queue_t* queue, HGE_list_t* link, unsigned int n);
void*        HGE_queue_pop_head(HGE_queue_t* queue);
HGE_list_t*  HGE_queue_pop_head_link(HGE_queue_t* queue);
HGE_list_t*  HGE_queue_peek_head_link(HGE_queue_t* queue);
HGE_list_t*  HGE_queue_peek_tail_link(HGE_queue_t* queue);
void*        HGE_queue_pop_tail(HGE_queue_t* queue);
void*        HGE_queue_pop_nth(HGE_queue_t* queue, unsigned int n);
HGE_list_t*  HGE_queue_pop_tail_link(HGE_queue_t* queue);
HGE_list_t*  HGE_queue_pop_nth_link(HGE_queue_t* queue, unsigned int n);
HGE_list_t*  HGE_queue_peek_nth_link(HGE_queue_t* queue, unsigned int n);
int          HGE_queue_link_index(HGE_queue_t* queue, HGE_list_t* link);
void         HGE_queue_unlink(HGE_queue_t* queue, HGE_list_t* link);
void         HGE_queue_delete_link(HGE_queue_t* queue, HGE_list_t* link);
void*        HGE_queue_peek_head(HGE_queue_t* queue);
void*        HGE_queue_peek_tail(HGE_queue_t* queue);
void*        HGE_queue_peek_nth(HGE_queue_t* queue, unsigned int n);
int          HGE_queue_index(HGE_queue_t* queue, const void* data);
void         HGE_queue_remove(HGE_queue_t* queue, const void* data);
void         HGE_queue_remove_all(HGE_queue_t* queue, const void* data);
void         HGE_queue_insert_before(HGE_queue_t* queue, HGE_list_t* sib, void* data);
void         HGE_queue_insert_after(HGE_queue_t* queue, HGE_list_t* sib, void* data);
void         HGE_queue_insert_sorted(HGE_queue_t* queue, HGE_cfunc_t func, void* data);
		
#endif
