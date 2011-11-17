#include <stdlib.h>
#include <string.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_mem.h"
#include "HGE_math.h"
#include "HGE_types.h"
#include "HGE_parray.h"

/* static inline functions */

static inline void
HGE_parray_maybe_expand(HGE_parray_t* array,
			   unsigned int length)
{
  unsigned int want_alloc;

  want_alloc = (array->length + length);
  if(want_alloc > array->alloc)
    {
      want_alloc   = HGE_math_nearest_power2(want_alloc);
      array->ptrs  = HGE_realloc(array->ptrs, void*, want_alloc);
      array->alloc = want_alloc;
    }

  return;
}

/* global functions */

HGE_error_t
HGE_parray_init(void* ptr)
{
  HGE_parray_t* array = ptr;

  array->ptrs   = NULL;
  array->length = 0;
  array->alloc  = 0;

  return HGE_SUCCESS;
}

void
HGE_parray_clean(void* ptr)
{
  HGE_free(((HGE_parray_t*)ptr)->ptrs);
}

void* 
HGE_parray_sized_alloc(unsigned int reserve_size)
{
  HGE_parray_t* array;

  if((array = HGE_alloc(HGE_parray_t)) != NULL)
    {
      HGE_parray_init(array);
      HGE_parray_set_size(array, reserve_size);
    }

   return array;
}

void*
HGE_parray_alloc(void)
{
  return HGE_parray_sized_alloc(0);
}


void
HGE_parray_free(void* ptr)
{
  HGE_parray_clean(ptr);
  HGE_free(ptr);
}

void
HGE_parray_add(HGE_parray_t* array, void* ptr)
{
  HGE_parray_maybe_expand(array, 1);
  array->ptrs[array->length++] = ptr;
}

void
HGE_parray_set(HGE_parray_t* array, void* ptr, unsigned int index)
{
  if(HGE_parray_length(array) < (index+1))
    HGE_parray_set_size(array, index+1);

  HGE_parray_index(array,index) = ptr;
}

void
HGE_parray_set_size(HGE_parray_t* array,
		    unsigned int length)
{
  int i;

  if(length > array->length)
    {
      HGE_parray_maybe_expand(array, (length - array->length));
      for(i = array->length; i < length; i++)
	array->ptrs[i] = NULL;
    }

  array->length = length;
}

void*
HGE_parray_remove_index(HGE_parray_t* array,
			   unsigned int index)
{
  void* rv;
  
  rv = array->ptrs[index];

  if(index != (array->length - 1))
    {
      memmove(array->ptrs + index,
	      array->ptrs + index + 1,
	      sizeof(void*) * (array->length - index - 1));
    }

  array->length--;

  return rv;
}

void*
HGE_parray_remove_index_fast(HGE_parray_t* array,
				unsigned int index)
{
  void* rv;

  rv = array->ptrs[index];

  if(index != (array->length - 1))
    array->ptrs[index] = array->ptrs[array->length - 1];

  array->length--;

  return rv;
}

void
HGE_parray_remove_range(HGE_parray_t* array,
			   unsigned int index,
			   unsigned int length)
{
  if((index + length) != array->length)
    {
      memmove(&array->ptrs[index],
	      &array->ptrs[index + length],
	      (array->length - (index + length)) * sizeof(void*));
    }

  array->length -= length;
}

int
HGE_parray_remove(HGE_parray_t* array,
		  void* ptr)
{
  unsigned int i;

  for(i = 0; i < array->length; i++)
    {
      if(array->ptrs[i] == ptr)
	{
	  HGE_parray_remove_index(array, i);
	  return HGE_TRUE;
	}
    }

  return HGE_FALSE;
}

int
HGE_parray_remove_fast(HGE_parray_t* array,
		       void* ptr)
{
  unsigned int i;

  for(i = 0; i < array->length; i++)
    {
      if(array->ptrs[i] == ptr)
	{
	  HGE_parray_remove_index_fast(array, i);
	  return HGE_TRUE;
	}
    }
  
  return HGE_FALSE;
}

void
HGE_parray_sort(HGE_parray_t* array,
		HGE_cfunc_t cmp_func)
{
  qsort(array->ptrs,
	array->length,
	sizeof(void*),
	cmp_func);
}

void
HGE_parray_foreach0(HGE_parray_t* array, HGE_func0_t func)
{
  int i;

  for(i = 0; i < array->length; i++)
    func(array->ptrs[i]);
}

void
HGE_parray_foreach1(HGE_parray_t* array, HGE_func1_t func, void* data)
{
  int i;

  for(i = 0; i < array->length; i++)
    func(array->ptrs[i], data);
}
		  



