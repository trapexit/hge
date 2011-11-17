#ifndef _HGE_PARRAY_H_
#define _HGE_PARRAY_H_

#include "HGE_basetypes.h"
#include "HGE_types.h"

#define HGE_parray_length(a) ((a)->length)
#define HGE_parray_index(a,i) (a)->ptrs[(i)]

typedef struct HGE_parray_t HGE_parray_t;
struct HGE_parray_t
{
  void**       ptrs;
  unsigned int length;
  unsigned int alloc;
};

HGE_error_t   HGE_parray_init(void* ptr);
void          HGE_parray_clean(void* ptr);
void*         HGE_parray_alloc(void);
void*         HGE_parray_sized_alloc(unsigned int reserve_size);
void          HGE_parray_free(void* ptr);
unsigned int  HGE_parray_get_length(HGE_parray_t* array);
void          HGE_parray_set_size(HGE_parray_t* array, unsigned int length);
void          HGE_parray_add(HGE_parray_t* array, void* ptr);
void          HGE_parray_set(HGE_parray_t* array, void* ptr, unsigned int index);
HGE_parray_t* HGE_parray_insert(HGE_parray_t* array, unsigned int index, const void* data, unsigned int length);
void*         HGE_parray_remove_index(HGE_parray_t* array, unsigned int index);
void*         HGE_parray_remove_index_fast(HGE_parray_t* array, unsigned int index);
void          HGE_parray_remove_range(HGE_parray_t* array, unsigned int index, unsigned int length);
void          HGE_parray_sort(HGE_parray_t* array, HGE_cfunc_t cmp_func);
void          HGE_parray_foreach0(HGE_parray_t* array, HGE_func0_t func);
void          HGE_parray_foreach1(HGE_parray_t* array, HGE_func1_t func, void* data);

#endif
