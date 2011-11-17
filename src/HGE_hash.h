#ifndef _HGE_HASH_H_
#define _HGE_HASH_H_

#include "HGE_typeinfo.h"

typedef struct HGE_hash_node_t HGE_hash_node_t;
struct HGE_hash_node_t
{
  void* key;
  void* value;
  HGE_hash_node_t* next;
};

typedef struct HGE_hash_t HGE_hash_t;
struct HGE_hash_t
{
  int               size;
  int               num_nodes;
  HGE_hash_node_t** nodes;
  HGE_hfunc_t       hash_func;
  HGE_efunc_t       key_equal_func;
  HGE_ffunc_t       key_free_func;
  HGE_ffunc_t       value_free_func;
};

extern HGE_typeinfo_t HGE_hash[];

/* HGE_hash allocation/initialize/clean/free */
void*  HGE_hash_alloc(void);
void*  HGE_hash_alloc_lite(HGE_hfunc_t hash_func,
			   HGE_efunc_t key_equal_func);
void*  HGE_hash_alloc_full(HGE_hfunc_t  hash_func, 
			   HGE_efunc_t key_equal_func, 
			   HGE_ffunc_t  key_free_func,
			   HGE_ffunc_t  value_free_func);
HGE_error_t HGE_hash_init(void* ptr);
HGE_error_t HGE_hash_init_lite(void* ptr,
			  HGE_hfunc_t hash_func,
			  HGE_efunc_t key_equal_func);
HGE_error_t HGE_hash_init_full(void* ptr,
			  HGE_hfunc_t hash_func,
			  HGE_efunc_t key_equal_func,
			  HGE_ffunc_t key_free_func,
			  HGE_ffunc_t value_free_func);
void   HGE_hash_clean(void* ptr);
void   HGE_hash_free(void* ptr);

/* HGE_hash functions */
HGE_error_t  HGE_hash_set_key_equal(HGE_hash_t* hash, HGE_efunc_t key_equal_func);
HGE_error_t  HGE_hash_set_key_free(HGE_hash_t* hash, HGE_ffunc_t key_free_func);
HGE_error_t  HGE_hash_set_value_free(HGE_hash_t* hash, HGE_ffunc_t value_free_func);
void*        HGE_hash_lookup(HGE_hash_t* hash, const void* key);
int          HGE_hash_lookup_extended(HGE_hash_t* hash, 
				      const void* lookup_key,
				      void**      orig_key, 
				      void**      value);
void*        HGE_hash_find0(HGE_hash_t* hash, HGE_hffunc0_t func); 
void*        HGE_hash_find1(HGE_hash_t* hash, HGE_hffunc1_t func, void* userdata); 
void         HGE_hash_insert(HGE_hash_t* hash, void* key, void* value);
void         HGE_hash_replace(HGE_hash_t* hash, void* key, void* value);
int          HGE_hash_remove(HGE_hash_t* hash, const void* key);
void         HGE_hash_remove_all(HGE_hash_t* hash);
int          HGE_hash_steal(HGE_hash_t* hash, const void* key);
void         HGE_hash_foreach0(HGE_hash_t* hash, HGE_hfunc0_t func);
void         HGE_hash_foreach1(HGE_hash_t* hash, HGE_hfunc1_t func, void* d);
unsigned int HGE_hash_size(HGE_hash_t* hash);

			    
#endif

