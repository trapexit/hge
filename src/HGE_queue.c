#include <stdlib.h>
#include "SDL.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_list.h"
#include "HGE_queue.h"

HGE_typeinfo_t HGE_queue[] = {{sizeof(HGE_queue_t),
			       HGE_queue_init,
			       HGE_queue_clean,
			       NULL,
			       NULL}};

HGE_error_t
HGE_queue_init(void* ptr)
{
  HGE_queue_t* queue = ptr;

  queue->length = 0;
  queue->head   = NULL;
  queue->tail   = NULL;

  return HGE_SUCCESS;
}

void
HGE_queue_clean(void* ptr)
{
  HGE_queue_t* queue = ptr;

  if(queue != NULL)
    {
      HGE_list_free(queue->head);
    }
}

void*
HGE_queue_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_queue_t)) != NULL)
    HGE_queue_init(ptr);

  return ptr;
}

void
HGE_queue_free(void* ptr)
{
  HGE_queue_clean(ptr);
  HGE_free(ptr);
}

int
HGE_queue_is_empty(HGE_queue_t* queue)
{
  return (queue->head == NULL);
}

unsigned int
HGE_queue_get_length(HGE_queue_t* queue)
{
  return queue->length;
}

void
HGE_queue_reverse(HGE_queue_t* queue)
{
  queue->tail = queue->head;
  queue->head = HGE_list_reverse(queue->head);
}

HGE_queue_t*
HGE_queue_copy(HGE_queue_t* queue)
{
  HGE_list_t*  list;
  HGE_queue_t* result;
  
  result = HGE_queue_alloc();
  
  list = queue->head;
  while(list != NULL)
    {
      HGE_queue_push_tail(result, list->data);
      list = list->next;
    }

  return result;
}

void
HGE_queue_push_head_queue(HGE_queue_t* queue0, HGE_queue_t* queue1)
{
  if(!queue0->length)
    {
      *queue0 = *queue1;
      return;
    }

  if(queue1->length)
    {
      queue1->tail->next = queue0->head;
      queue0->head = queue1->head;
      queue0->length += queue1->length;
    }
}

void
HGE_queue_push_tail_queue(HGE_queue_t* queue0, HGE_queue_t* queue1)
{
  if(!queue0->length)
    {
      *queue0 = *queue1;
      return;
    }

  if(queue1->length)
    {
      queue0->tail->next = queue1->head;
      queue0->tail = queue1->tail;
      queue0->length += queue1->length;
    }
}

void
HGE_queue_foreach0(HGE_queue_t* queue,
		   HGE_func0_t  func)
{
  HGE_list_foreach0(queue->head, func);
}

void
HGE_queue_foreach1(HGE_queue_t* queue,
		   HGE_func1_t  func,
		   void*        user_data)
{
  HGE_list_foreach1(queue->head, func, user_data);
}

HGE_list_t*
HGE_queue_find(HGE_queue_t* queue, const void* data)
{
  return HGE_list_find(queue->head, data);
}

HGE_list_t*
HGE_queue_find_custom(HGE_queue_t* queue,
		      HGE_cfunc_t func,
		      const void* data)
{
  return HGE_list_find_custom(queue->head, func, data);
}

void
HGE_queue_push_head(HGE_queue_t* queue, void* data)
{
  queue->head = HGE_list_prepend(queue->head, data);
  if(queue->tail == NULL)
    queue->tail = queue->head;
  queue->length++;
}

void
HGE_queue_push_nth(HGE_queue_t* queue, void* data, unsigned int n)
{
  if(n >= queue->length)
    HGE_queue_push_tail(queue, data);
  else
    HGE_queue_insert_before(queue, HGE_queue_peek_nth_link(queue,n), data);
}

void
HGE_queue_push_head_link(HGE_queue_t* queue, HGE_list_t* link)
{
  link->next = queue->head;
  if(queue->head != NULL)
    queue->head->prev = link;
  else
    queue->tail = link;
  queue->head = link;
  queue->length++;
}

void
HGE_queue_push_tail(HGE_queue_t* queue, void* data)
{
  queue->tail = HGE_list_append(queue->tail, data);
  if(queue->tail->next)
    queue->tail = queue->tail->next;
  else
    queue->head = queue->tail;
  queue->length++;
}

void
HGE_queue_push_tail_link(HGE_queue_t* queue, HGE_list_t* link)
{
  link->prev = queue->tail;
  if (queue->tail)
    queue->tail->next = link;
  else
    queue->head = link;
  queue->tail = link;
  queue->length++;
}
void
HGE_queue_push_nth_link(HGE_queue_t* queue, HGE_list_t* link, unsigned int n)
{
  HGE_list_t* next;
  HGE_list_t* prev;

  if(n >= queue->length)
    HGE_queue_push_tail_link(queue, link);
  else
    {
      next = HGE_queue_peek_nth_link (queue, n);
      prev = next->prev;
      
      if(prev)
	prev->next = link;
      next->prev = link;
      
      link->next = next;
      link->prev = prev;
      
      if(queue->head->prev)
	queue->head = queue->head->prev;
      
      if(queue->tail->next)
	queue->tail = queue->tail->next;
      
      queue->length++;
    }
}

void*
HGE_queue_pop_head(HGE_queue_t* queue)
{
  void* data;
  HGE_list_t* node;

  if(queue->head != NULL)
    {
      node = queue->head;
      data = node->data;
      
      queue->head = node->next;
      if(queue->head != NULL)
	queue->head->prev = NULL;
      else
	queue->tail = NULL;
      
      HGE_list_free_link(node);
      queue->length--;

      return data;
    }

  return NULL;
}

HGE_list_t*
HGE_queue_pop_head_link(HGE_queue_t* queue)
{
  HGE_list_t* node;

  if(queue->head != NULL)
    {
      node = queue->head;
      
      queue->head = node->next;
      if(queue->head != NULL)
        {
          queue->head->prev = NULL;
          node->next = NULL;
        }
      else
        queue->tail = NULL;
      queue->length--;

      return node;
    }

  return NULL;
}

HGE_list_t*
HGE_queue_peek_head_link(HGE_queue_t* queue)
{
  return queue->head;
}

HGE_list_t*
HGE_queue_peek_tail_link(HGE_queue_t* queue)
{
  return queue->tail;
}

void*
HGE_queue_pop_tail(HGE_queue_t* queue)
{
  void* data;
  HGE_list_t* node;

  if(queue->tail != NULL)
    {
      node = queue->tail;
      data = node->data;

      queue->tail = node->prev;
      if(queue->tail != NULL)
        queue->tail->next = NULL;
      else
        queue->head = NULL;
      queue->length--;
      HGE_list_free_link(node);

      return data;
    }
  
  return NULL;

}

void*
HGE_queue_pop_nth(HGE_queue_t* queue, unsigned int n)
{
  void* result;
  HGE_list_t* nth_link;
  
  if(n >= queue->length)
    return NULL;

  nth_link = HGE_queue_peek_nth_link(queue, n);
  result = nth_link->data;

  HGE_queue_delete_link(queue, nth_link);

  return result;
}

HGE_list_t*
HGE_queue_pop_tail_link(HGE_queue_t* queue)
{
  HGE_list_t* node;

  if(queue->tail != NULL)
    {
      node = queue->tail;

      queue->tail = node->prev;
      if(queue->tail != NULL)
	{
	  queue->tail->next = NULL;
	  node->prev = NULL;
	}
      else
	queue->head = NULL;
      queue->length--;

      return node;
    }

  return NULL;
}

HGE_list_t*
HGE_queue_pop_nth_link(HGE_queue_t* queue, unsigned int n)
{
  HGE_list_t* link;

  if(n >= queue->length)
    return NULL;

  link = HGE_queue_peek_nth_link(queue, n);
  HGE_queue_unlink(queue, link);

  return link;
}

HGE_list_t*
HGE_queue_peek_nth_link(HGE_queue_t* queue, unsigned int n)
{
  int i;
  HGE_list_t* link;

  if(n >= queue->length)
    return NULL;

  if(n > (queue->length >> 1))
    {
      n = (queue->length - n - 1);
      
      link = queue->tail;
      for(i = 0; i < n; i++)
	link = link->prev;
    }
  else
    {
      link = queue->head;
      for(i = 0; i < n; i++)
	link = link->next;
    }

  return link;
}

int
HGE_queue_link_index(HGE_queue_t* queue, HGE_list_t* link)
{
  return HGE_list_position(queue->head, link);  
}

void
HGE_queue_unlink(HGE_queue_t* queue, HGE_list_t* link)
{
  if(link == queue->tail)
    queue->tail = queue->tail->prev;

  queue->head = HGE_list_remove_link(queue->head, link);
  queue->length--;
}

void
HGE_queue_delete_link(HGE_queue_t* queue, HGE_list_t* link)
{
  HGE_queue_unlink(queue, link);
  HGE_list_free(link);
}

void*
HGE_queue_peek_head(HGE_queue_t* queue)
{
  return ((queue->head != NULL) ? queue->head->data : NULL);
}

void*
HGE_queue_peek_tail(HGE_queue_t* queue)
{
  return ((queue->tail != NULL) ? queue->tail->data : NULL);
}

void*
HGE_queue_peek_nth(HGE_queue_t* queue, unsigned int n)
{
  HGE_list_t* link;

  link = HGE_queue_peek_nth_link(queue, n);

  return ((link != NULL) ? link->data : NULL);
}

int
HGE_queue_index(HGE_queue_t* queue, const void* data)
{
  return HGE_list_index(queue->head, data);
}

void
HGE_queue_remove(HGE_queue_t* queue, const void* data)
{
  HGE_list_t* link;

  link = HGE_list_find(queue->head, data);
  
  if(link != NULL)
    HGE_queue_delete_link(queue, link);
}

void
HGE_queue_remove_all(HGE_queue_t* queue, const void* data)
{
  HGE_list_t* list;
  HGE_list_t* next;

  list = queue->head;
  while(list != NULL)
    {
      next = list->next;
      
      if(list->data == data)
	HGE_queue_delete_link(queue, list);
      
      list = next;
    }
}

void
HGE_queue_insert_before(HGE_queue_t* queue,
			HGE_list_t*  sib,
			void*        data)
{
  if(sib == NULL)
    return;

  queue->head = HGE_list_insert_before(queue->head, sib, data);
  queue->length++;
}

void
HGE_queue_insert_after(HGE_queue_t* queue,
		       HGE_list_t*  sib,
		       void*        data)
{
  if(sib == queue->tail)
    HGE_queue_push_tail(queue, data);
  else
    HGE_queue_insert_before(queue, sib->next, data);
}

void
HGE_queue_insert_sorted(HGE_queue_t* queue,
			HGE_cfunc_t  func,
			void*        data)
{
  HGE_list_t* list;

  list = queue->head;
  while(list != NULL && func(list->data, data) < 0)
    list = list->next;

  if(list != NULL)
    HGE_queue_insert_before(queue, list, data);
  else
    HGE_queue_push_tail(queue, data);
}
