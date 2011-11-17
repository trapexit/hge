#include <stdlib.h>
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_slist.h"
#include "HGE_trash_stack.h"

/* global functions */

void
HGE_slist_init(HGE_slist_t* slist)
{
  slist->next = NULL;
  slist->data = NULL;
}

HGE_slist_t*
HGE_slist_alloc(void)
{
  HGE_slist_t* slist;

  if((slist = HGE_alloc(HGE_slist_t)) != NULL)
    HGE_slist_init(slist);

  return slist;
}

void
HGE_slist_free(HGE_slist_t* slist)
{
  HGE_slist_t* prev;

  while(slist != NULL)
    {
      prev  = slist;
      slist = slist->next;
      HGE_free(slist);
    }
}

void
HGE_slist_free_link(HGE_slist_t* slist)
{
  HGE_free(slist);
}

HGE_slist_t*
HGE_slist_append(HGE_slist_t* slist, void* data)
{
  HGE_slist_t* new;
  HGE_slist_t* last;

  new = HGE_slist_alloc();
  new->data = data;
  
  if(slist != NULL)
    {
      last = HGE_slist_last(slist);
      last->next = new;

      return slist;
    }

  return new;
}

HGE_slist_t*
HGE_slist_prepend(HGE_slist_t* slist, void* data)
{
  HGE_slist_t* new;

  new = HGE_slist_alloc();
  new->data = data;
  new->next = slist;

  return new;
}

HGE_slist_t*
HGE_slist_insert(HGE_slist_t* slist, void* data, unsigned int pos)
{
  HGE_slist_t* prev;
  HGE_slist_t* tmp;
  HGE_slist_t* new;

  new = HGE_slist_alloc();
  new->data = data;

  if(slist == NULL)
    return new;

  prev = NULL;
  tmp = slist;
  
  while((pos-- != 0) && (tmp != NULL))
    {
      prev = tmp;
      tmp = tmp->next;
    }

  if(prev != NULL)
    {
      new->next  = prev->next;
      prev->next = new;
    }
  else
    {
      new->next = slist;
      slist = new;
    }

  return slist;
}

HGE_slist_t*
HGE_slist_insert_before(HGE_slist_t* slist,
			HGE_slist_t* sib,
			void* data)
{
  if(slist == NULL)
    {
      slist = HGE_slist_alloc();
      slist->data = data;
      return slist;
    }
  else
    {
      HGE_slist_t* node;
      HGE_slist_t* prev = NULL;

      for(node = slist; slist; prev = node, node = prev->next)
        if (node == sib)
          break;

      if (prev == NULL)
        {
          node = HGE_slist_alloc();
          node->data = data;
          node->next = slist;

          return node;
        }
      else
        {
          node = HGE_slist_alloc();
          node->data = data;
          node->next = prev->next;
          prev->next = node;

          return slist;
        }
    }
}

HGE_slist_t*
HGE_slist_concat(HGE_slist_t* slist0, HGE_slist_t* slist1)
{
  if(slist1 != NULL)
    {
      if(slist0 != NULL)
	(HGE_slist_last(slist0))->next = slist1;
      else
	slist0 = slist1;
    }

  return slist0;
}

HGE_slist_t*
HGE_slist_remove(HGE_slist_t* slist, const void* data)
{
  HGE_slist_t* tmp;
  HGE_slist_t* prev = NULL;

  tmp = slist;
  while(tmp != NULL)
    {
      if(tmp->data == data)
	{
	  if(prev != NULL)
	    prev->next = tmp->next;
	  else
	    slist = tmp->next;
	  
	  HGE_slist_free_link(tmp);
	  break;
	}
      
      prev = tmp;
      tmp = prev->next;
    }

  return slist;
}

HGE_slist_t*
HGE_slist_remove_all(HGE_slist_t* slist, const void* data)
{
  HGE_slist_t* tmp;
  HGE_slist_t* next;
  HGE_slist_t* prev = NULL;

  tmp = slist;
  while(tmp != NULL)
    {
      if(tmp->data == data)
	{
	  next = tmp->next;
	  
	  if(prev != NULL)
	    prev->next = next;
	  else
	    slist = next;
	  
	  HGE_slist_free_link(tmp);
	  tmp = next;
	}
      else
	{
	  prev = tmp;
	  tmp = prev->next;
	}
    }

  return slist;
}

HGE_slist_t*
HGE_slist_remove_link(HGE_slist_t* slist, HGE_slist_t* link)
{
  HGE_slist_t* tmp;
  HGE_slist_t* prev;

  prev = NULL;
  tmp  = slist;

  while(tmp != NULL)
    {
      if(tmp == link)
	{
	  if(prev != NULL)
	    prev->next = tmp->next;
	  if(slist == tmp)
	    slist = slist->next;
	  
	  tmp->next = NULL;
	  break;
	}

      prev = tmp;
      tmp = tmp->next;
    }

  return slist;
}

HGE_slist_t*
HGE_slist_delete_link(HGE_slist_t* slist, HGE_slist_t* link)
{
  slist = HGE_slist_remove_link(slist, link);
  HGE_slist_free_link(link);

  return slist;
}

HGE_slist_t*
HGE_slist_copy(HGE_slist_t* slist)
{
  HGE_slist_t* prev;
  HGE_slist_t* new = NULL;

  if(slist != NULL)
    {
      new = HGE_slist_alloc();
      new->data = slist->data;
      prev = new;
      slist = slist->next;
      while(slist != NULL)
	{
	  prev->next = HGE_slist_alloc();
	  prev = prev->next;
	  prev->data = slist->data;
	  slist = slist->next;
	}
    }

  return new;
}

HGE_slist_t*
HGE_slist_reverse(HGE_slist_t* slist)
{
  HGE_slist_t* next;
  HGE_slist_t* prev = NULL;
  
  while(slist != NULL)
    {
      next = slist->next;
      slist->next = prev;
      prev = slist;
      slist = next;
    }

  return prev;
}

HGE_slist_t*
HGE_slist_nth(HGE_slist_t* slist, unsigned int n)
{
  while((n-- > 0) && (slist != NULL))
    slist = slist->next;

  return slist;
}

void*
HGE_slist_nth_data(HGE_slist_t* slist, unsigned int n)
{
  while((n-- > 0) && (slist != NULL))
    slist = slist->next;

  return (slist ? slist->data : NULL);
}

HGE_slist_t*
HGE_slist_find(HGE_slist_t* slist, void* data)
{
  while(slist != NULL)
    {
      if(slist->data == data)
	break;

      slist = slist->next;
    }

  return slist;
}

int
HGE_slist_position(HGE_slist_t* slist, HGE_slist_t* link)
{
  int i;

  i = 0;
  while(slist != NULL)
    {
      if(slist == link)
	return i;
      i++;
      slist = slist->next;
    }
  
  return -1;
}

int
HGE_slist_index(HGE_slist_t* slist, const void* data)
{
  int i = 0;

  while(slist != NULL)
    {
      if(slist->data == data)
	return i;
      i++;
      slist = slist->next;
    }

  return -1;
}

HGE_slist_t*
HGE_slist_last(HGE_slist_t* slist)
{
  if(slist != NULL)
    {
      while(slist->next != NULL)
	slist = slist->next;
    }

  return slist;
}

int
HGE_slist_length(HGE_slist_t* slist)
{
  int length = 0;

  while(slist != NULL)
    {
      length++;
      slist = slist->next;
    }

  return length;
}

void
HGE_slist_foreach0(HGE_slist_t* slist, HGE_func0_t func)
{
  while(slist != NULL)
    {
      func(slist->data);
      slist = slist->next;
    }
}

void
HGE_slist_foreach1(HGE_slist_t* slist, 
		   HGE_func1_t  func, 
		   void*        data)
{
  while(slist != NULL)
    {
      func(slist->data, data);
      slist = slist->next;
    }
}

HGE_slist_t*
HGE_slist_insert_sorted(HGE_slist_t* slist,
			HGE_cfunc_t  func,
			void*        data)
{
  int cmp;
  HGE_slist_t* tmp_list = slist;
  HGE_slist_t* prev_list = NULL;
  HGE_slist_t* new_list;

  if(func == NULL)
    return slist;

  if(slist == NULL)
    {
      new_list = HGE_slist_alloc();
      new_list->data = data;
      return new_list;
    }
 
  cmp = func(data, tmp_list->data);
  while((tmp_list->next) && (cmp > 0))
    {
      prev_list = tmp_list;
      tmp_list = tmp_list->next;
      cmp = func(data, tmp_list->data);
    }

  new_list = HGE_slist_alloc();
  new_list->data = data;

  if((tmp_list->next == NULL) && (cmp > 0))
    {
      tmp_list->next = new_list;
      return slist;
    }
  
  if(prev_list != NULL)
    {
      prev_list->next = new_list;
      new_list->next = tmp_list;
      return slist;
    }
  else
    {
      new_list->next = slist;
      return new_list;
    }
}
