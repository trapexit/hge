#include <stdlib.h>
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_list.h"
#include "HGE_trash_stack.h"

HGE_list_t*
HGE_list_alloc(void)
{
  HGE_list_t* list;

  if((list = HGE_alloc(HGE_list_t)) != NULL)
    HGE_list_init(list);
  
  return list;
}

void
HGE_list_init(HGE_list_t* list)
{
  list->next = NULL;
  list->prev = NULL;
  list->data = NULL;
}

void
HGE_list_free(HGE_list_t* list)
{
  HGE_list_t* prev;

  while(list != NULL)
    {
      prev = list;
      list = list->next;
      HGE_free(prev);
    }
}

void
HGE_list_free_link(HGE_list_t* list)
{
  HGE_free(list);
}

HGE_list_t*
HGE_list_append(HGE_list_t* list, void* data)
{
  HGE_list_t* new;
  HGE_list_t* last;

  new = HGE_list_alloc();
  new->data = data;
  if(list != NULL)
    {
      last = HGE_list_last(list);
      last->next = new;
      new->prev = last;

      return list;
    }

  return new;
}

HGE_list_t*
HGE_list_prepend(HGE_list_t* list, void* data)
{
  HGE_list_t* new;

  new = HGE_list_alloc();
  new->data = data;
  
  if(list != NULL)
    {
      if(list->prev != NULL)
	{
	  list->prev->next = new;
	  new->prev = list->prev;
	}
      list->prev = new;
      new->next = list;
    }

  return new;
}

HGE_list_t*
HGE_list_insert(HGE_list_t* list, void* data, int pos)
{
  HGE_list_t* new_list;
  HGE_list_t* tmp_list;

  if(pos < 0)
    return HGE_list_append(list, data);
  else if(pos == 0)
    return HGE_list_prepend(list, data);
  
  tmp_list = HGE_list_nth(list, pos);
  if(tmp_list == NULL)
    return HGE_list_append(list, data);
  
  new_list = HGE_list_alloc();
  new_list->data = data;
  
  if(tmp_list->prev != NULL)
    {
      tmp_list->prev->next = new_list;
      new_list->prev = tmp_list->prev;
    }

  new_list->next = tmp_list;
  tmp_list->prev = new_list;
  
  if(tmp_list == list)
    return new_list;
  else
    return list;
}

HGE_list_t* HGE_list_insert_before(HGE_list_t* list,
				   HGE_list_t* sib,
				   void*       data)
{
  if(list == NULL)
    {
      list = HGE_list_alloc();
      list->data = data;
      return list;
    }
  else if(sib != NULL)
    {
      HGE_list_t* node;

      node = HGE_list_alloc();
      node->data = data;
      if(sib->prev != NULL)
        {
          node->prev = sib->prev;
          node->prev->next = node;
          node->next = sib;
          sib->prev = node;
          return list;
        }
      else
        {
          node->next = sib;
          sib->prev = node;
          return node;
        }
    }
  else
    {
      HGE_list_t* last;
      
      last = list;
      while(last->next != NULL)
	last = last->next;
      
      last->next = HGE_list_alloc();
      last->next->data = data;
      last->next->prev = last;
      
      return list;
    }
}

HGE_list_t*
HGE_list_concat(HGE_list_t* list0, HGE_list_t* list1)
{
  HGE_list_t* tmp_list;

  if(list1)
    {
      tmp_list = HGE_list_last(list0);
      if(tmp_list != NULL)
	tmp_list->next = list1;
      else
	list0 = list1;
      list1->prev = tmp_list;
    }

  return list0;
}

HGE_list_t*
HGE_list_remove(HGE_list_t* list, void* data)
{
  HGE_list_t* tmp;

  tmp = list;
  while(tmp != NULL)
    {
      if(tmp->data != data)
	tmp = tmp->next;
      else
	{
	  if(tmp->prev != NULL)
	    tmp->prev->next = tmp->next;
	  if(tmp->next != NULL)
	    tmp->next->prev = tmp->prev;
	  if(list == tmp)
	    list = list->next;
	  
	  HGE_list_free_link(tmp);
	  break;
	}
    }

  return list;
}

HGE_list_t*
HGE_list_remove_link(HGE_list_t* list, HGE_list_t* link)
{
  if(link != NULL)
    {
      if(link->prev != NULL)
        link->prev->next = link->next;
      if(link->next != NULL)
        link->next->prev = link->prev;
      
      if(link == list)
        list = list->next;
      
      link->next = NULL;
      link->prev = NULL;
    }
  
  return list;
}

HGE_list_t*
HGE_list_delete_link(HGE_list_t* list, HGE_list_t* link)
{
  list = HGE_list_remove_link(list, link);
  HGE_list_free_link(link);
  
  return list;
}

HGE_list_t*
HGE_list_copy(HGE_list_t* list)
{
  HGE_list_t* new_list = NULL;

  if(list != NULL)
    {
      HGE_list_t* last;

      new_list = HGE_list_alloc();
      new_list->data = list->data;
      last = new_list;
      list = list->next;
      while(list != NULL)
        {
          last->next = HGE_list_alloc();
          last->next->prev = last;
          last = last->next;
          last->data = list->data;
          list = list->next;
        }
    }

  return new_list;
}

HGE_list_t*
HGE_list_reverse(HGE_list_t* list)
{
  HGE_list_t* last = NULL;

  while(list != NULL)
    {
      last = list;
      list = last->next;
      last->next = last->prev;
      last->prev = list;
    }

  return last;
}

HGE_list_t*
HGE_list_nth(HGE_list_t* list, unsigned int n)
{
  while((n-- > 0) && (list != NULL))
    list = list->next;

  return list;
}

HGE_list_t*
HGE_list_nth_prev(HGE_list_t* list, unsigned int n)
{
  while((n-- > 0) && (list != NULL))
    list = list->prev;

  return list;
}

void*
HGE_list_nth_data(HGE_list_t* list, unsigned int n)
{
  while((n-- > 0) && (list != NULL))
    list = list->next;

  return (list ? list->data : NULL);
}

HGE_list_t*
HGE_list_find(HGE_list_t* list, const void* data)
{
  while(list != NULL)
    {
      if(list->data == data)
	break;

      list = list->next;
    }

  return list;
}

HGE_list_t*
HGE_list_find_custom(HGE_list_t* list,
		     HGE_cfunc_t func,
		     const void* data)
{
  if(func == NULL)
    return list;

  while(list != NULL)
    {
      if(!func(list->data, data))
	return list;
      list = list->next;
    }

  return NULL;
}

int
HGE_list_position(HGE_list_t* list, HGE_list_t* link)
{
  int i = 0;

  while(list != NULL)
    {
      if(list == link)
	return i;
      i++;
      list = list->next;
    }

  return -1;
}

int
HGE_list_index(HGE_list_t* list, const void* data)
{
  int i = 0;

  while(list != NULL)
    {
      if(list->data == data)
	return i;
      i++;
      list = list->next;
    }

  return -1;
}

HGE_list_t*
HGE_list_first(HGE_list_t* list)
{
  if(list != NULL)
    {
      while(list->prev != NULL)
	list = list->prev;
    }

  return list;
}

HGE_list_t*
HGE_list_last(HGE_list_t* list)
{
  if(list != NULL)
    {
      while(list->next != NULL)
	list = list->next;
    }

  return list;
}

int
HGE_list_length(HGE_list_t* list)
{
  int length = 0;

  while(list != NULL)
    {
      length++;
      list = list->next;
    }

  return length;
}

void
HGE_list_foreach0(HGE_list_t* list,
		  HGE_func0_t func)
{
  while(list != NULL)
    {
      func(list->data);
      list = list->next;
    }
}

void
HGE_list_foreach1(HGE_list_t* list, 
		  HGE_func1_t func, 
		  void* user_data)
{
  while(list != NULL)
    {
      func(list->data, user_data);
      list = list->next;
    }
}

HGE_list_t*
HGE_list_insert_sorted(HGE_list_t* list,
		       HGE_cfunc_t func,
		       void*       data)
{
  int cmp;
  HGE_list_t* new_list;
  HGE_list_t* tmp_list = list;

  if(func == NULL)
    return list;

  if(list == NULL)
    {
      new_list = HGE_list_alloc();
      new_list->data = data;
      return new_list;
    }
  
  cmp = func(data, tmp_list->data);
  while((tmp_list->next != NULL) && (cmp > 0))
    {
      tmp_list = tmp_list->next;
      cmp = func(data, tmp_list->data);
    }

  new_list = HGE_list_alloc();
  new_list->data = data;

  if((tmp_list->next == NULL) && (cmp > 0))
    {
      tmp_list->next = new_list;
      new_list->prev = tmp_list;
      return list;
    }

  if(tmp_list->prev != NULL)
    {
      tmp_list->prev->next = new_list;
      new_list->prev = tmp_list->prev;
    }
  new_list->next = tmp_list;
  tmp_list->prev = new_list;

  if(tmp_list == list)
    return new_list;
  else
    return list;
  
}
