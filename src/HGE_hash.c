#include <stdlib.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_basetypes.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_math.h"
#include "HGE_hash.h"

#define HASH_MIN_SIZE 11
#define HASH_MAX_SIZE 13845163

HGE_typeinfo_t HGE_hash[] = {{sizeof(HGE_hash_t),
			      HGE_hash_init,
			      HGE_hash_clean,
			      NULL,
			      NULL}};

/* inline static functions */
static unsigned int
hge_hash_default_hash(const void* key)
{
  return (unsigned int)key;
}

static inline void
hge_hash_resize(HGE_hash_t* hash)
{
  int i;
  unsigned int new_size;
  unsigned int hash_val;
  HGE_hash_node_t*  node;
  HGE_hash_node_t*  next;
  HGE_hash_node_t** new_nodes;

  new_size = HGE_math_nearest_prime(hash->num_nodes);
  new_size = CLAMP_NUMBER(new_size, HASH_MIN_SIZE, HASH_MAX_SIZE);

  new_nodes = HGE_allocV(HGE_hash_node_t*, new_size);
  
  for(i = 0; i < hash->size; i++)
    {
      node = hash->nodes[i];
      while(node != NULL)
	{
	  next = node->next;

	  hash_val = (hash->hash_func(node->key) % new_size);

	  node->next = new_nodes[hash_val];
	  new_nodes[hash_val] = node;
	  
	  node = next;
	}
    }

  HGE_free(hash->nodes);
  hash->nodes = new_nodes;
  hash->size  = new_size;
}

static inline void
hge_hash_maybe_resize(HGE_hash_t* hash)
{
  if(((hash->size >= (3 * hash->num_nodes)) &&
      (hash->size > HASH_MIN_SIZE)) ||
     ((hash->num_nodes >= (3 * hash->size)) &&
      (hash->size < HASH_MAX_SIZE)))
    hge_hash_resize(hash);
}

static inline HGE_hash_node_t**
hge_hash_lookup_node(HGE_hash_t* hash,
		     const void* key)
{
  HGE_hash_node_t** node;
  
  node = &hash->nodes[hash->hash_func(key) % hash->size];
  
  if(hash->key_equal_func == NULL)
    {
      while((*node != NULL) && ((*node)->key != key))
	node = &(*node)->next;
    }
  else
    {
      while((*node != NULL) && !hash->key_equal_func((*node)->key, key))
	node = &(*node)->next;
    }
  
  return node;
}

static inline HGE_hash_node_t*
hge_hash_node_alloc(void* key, void* value)
{
  HGE_hash_node_t* node;

  if((node = HGE_alloc(HGE_hash_node_t)) != NULL)
    {
      node->key   = key;
      node->value = value;
      node->next  = NULL;
    }

  return node;
}

static inline void
hge_hash_node_free(HGE_hash_node_t* node,
		   HGE_ffunc_t key_free_func,
		   HGE_ffunc_t value_free_func)
{
  if(key_free_func != NULL)
    key_free_func(node->key);
  if(value_free_func != NULL)
    value_free_func(node->value);

  HGE_free(node);
}

static inline void
hge_hash_nodes_free(HGE_hash_node_t* node,
		    HGE_ffunc_t key_free_func,
		    HGE_ffunc_t value_free_func)
{
  HGE_hash_node_t* next;

  while(node != NULL)
    {
      next = node->next;
      hge_hash_node_free(node, key_free_func, value_free_func);
      node = next;
    }
}

/* global functions */
unsigned int
HGE_hash_size(HGE_hash_t* hash)
{
  return hash->num_nodes;
}

HGE_error_t
HGE_hash_set_hash(HGE_hash_t* hash, HGE_hfunc_t hash_func)
{
  if(HGE_hash_size(hash) != 0)
    return HGE_HASH_IN_USE;

  hash->hash_func = hash_func;

  return HGE_OK;
}

HGE_error_t
HGE_hash_set_key_equal(HGE_hash_t* hash, HGE_efunc_t key_equal_func)
{
  if(HGE_hash_size(hash) != 0)
    return HGE_HASH_IN_USE;

  hash->key_equal_func = key_equal_func;

  return HGE_OK;
}

HGE_error_t
HGE_hash_set_key_free(HGE_hash_t* hash, HGE_ffunc_t key_free_func)
{
  if(HGE_hash_size(hash) != 0)
    return HGE_HASH_IN_USE;

  hash->key_free_func = key_free_func;

  return HGE_OK;
}

HGE_error_t
HGE_hash_set_value_free(HGE_hash_t* hash, HGE_ffunc_t value_free_func)
{
  if(HGE_hash_size(hash) != 0)
    return HGE_HASH_IN_USE;

  hash->value_free_func = value_free_func;

  return HGE_OK;
}

HGE_error_t
HGE_hash_init_full(void* ptr,
		   HGE_hfunc_t hash_func,
		   HGE_efunc_t key_equal_func,
		   HGE_ffunc_t key_free_func,
		   HGE_ffunc_t value_free_func) 
{
  HGE_hash_t* hash = ptr;
  unsigned int i;

  hash->size = HASH_MIN_SIZE;
  hash->num_nodes = 0;
  hash->hash_func = ((hash_func == NULL)?hge_hash_default_hash:hash_func);
  HGE_hash_set_key_equal(hash, key_equal_func);
  HGE_hash_set_key_free(hash, key_free_func);
  HGE_hash_set_value_free(hash, value_free_func);
  hash->nodes = HGE_allocV(HGE_hash_node_t*, hash->size);
  if(hash->nodes == NULL)
    return HGE_HASH_INIT_FAILED;
  
  for(i = 0; i < hash->size; i++)
    hash->nodes[i] = NULL;

  return HGE_SUCCESS;
}

HGE_error_t
HGE_hash_init_lite(void* ptr,
		   HGE_hfunc_t  hash_func, 
		   HGE_efunc_t  key_equal_func)
{
  return HGE_hash_init_full(ptr, hash_func, key_equal_func, NULL, NULL);
}

HGE_error_t
HGE_hash_init(void* ptr)
{
  return HGE_hash_init_full(ptr, NULL, NULL, NULL, NULL);
}

void*
HGE_hash_alloc_full(HGE_hfunc_t hash_func,
		    HGE_efunc_t key_equal_func,
		    HGE_ffunc_t key_free_func,
		    HGE_ffunc_t value_free_func)
{
  HGE_hash_t* hash;

  if((hash = HGE_alloc(HGE_hash_t)) != NULL)
    {
      if(HGE_hash_init_full(hash, hash_func, key_equal_func, key_free_func, value_free_func))
	{
	  HGE_free_and_null(hash);
	}
    }

  return hash;
}

void*
HGE_hash_alloc_lite(HGE_hfunc_t hash_func, HGE_efunc_t key_equal_func)
{
  return HGE_hash_alloc_full(hash_func, key_equal_func, NULL, NULL);
}

void*
HGE_hash_alloc(void)
{
  return HGE_hash_alloc_full(NULL, NULL, NULL, NULL);
}

void
HGE_hash_clean(void* ptr)
{
  HGE_hash_t* hash = ptr;

  if(hash != NULL)
    {
      HGE_hash_remove_all(hash);
      HGE_free(hash->nodes);
    }
}

void
HGE_hash_free(void* ptr)
{
  HGE_hash_clean(ptr);
  HGE_free(ptr);
}

void*
HGE_hash_lookup(HGE_hash_t* hash, const void* key)
{
  HGE_hash_node_t* node;

  node = *hge_hash_lookup_node(hash, key);

  return (node != NULL) ? node->value : NULL;
}

int
HGE_hash_lookup_extended(HGE_hash_t* hash,
			 const void* lookup_key,
			 void** orig_key,
			 void** value)
{
  HGE_hash_node_t* node;

  node = *hge_hash_lookup_node(hash, lookup_key);
  if(node != NULL)
    {
      if(orig_key != NULL)
	*orig_key = node->key;
      if(value != NULL)
	*value = node->value;
      return HGE_TRUE;
    }

  return HGE_FALSE;
}

void*
HGE_hash_find0(HGE_hash_t* hash, HGE_hffunc0_t func)
{
  HGE_uint_t i;
  HGE_hash_node_t* node;

  for(i = 0; i < hash->size; i++)
    {
      node = hash->nodes[i];
      while(node != NULL)
	{
	  if(func(node->key, node->value))
	    return node->value;
	  node = node->next;
	}
    }
  return NULL;
}

void*
HGE_hash_find1(HGE_hash_t* hash, HGE_hffunc1_t func, void* userdata)
{
  HGE_uint_t i;
  HGE_hash_node_t* node;
  HGE_hash_node_t* next;

  for(i = 0; i < hash->size; i++)
    {
      node = hash->nodes[i];
      while(node != NULL)
	{
	  next = node->next;
	  if(func(node->key, node->value, userdata))
	    return node->value;
	  node = next;
	}
    }
  return NULL;
}

void
HGE_hash_insert(HGE_hash_t* hash,
		void*       key,
		void*       value)
{
  HGE_hash_node_t** node;

  node = hge_hash_lookup_node(hash, key);
  if(*node != NULL)
    {
      if((key != (*node)->key) &&
	 (hash->key_free_func != NULL))
	hash->key_free_func(key);
      
      if((value != (*node)->value) &&
	 (hash->value_free_func != NULL))
	hash->value_free_func((*node)->value);
      
      (*node)->value = value;
    }
  else
    {
      *node = hge_hash_node_alloc(key, value);
      hash->num_nodes++;
      hge_hash_maybe_resize(hash);
    }
}

void
HGE_hash_replace(HGE_hash_t* hash,
		 void*       key,
		 void*       value)
{
  HGE_hash_node_t** node;

  node = hge_hash_lookup_node(hash, key);
  if(*node != NULL)
    {
      if((key != (*node)->key) &&
	 (hash->key_free_func != NULL))
	hash->key_free_func((*node)->key);
      if((value != (*node)->value) && 
	 (hash->value_free_func != NULL))
	hash->value_free_func((*node)->value);
      
      (*node)->key   = key;
      (*node)->value = value;
    }
  else
    {
      *node = hge_hash_node_alloc(key, value);
      hash->num_nodes++;
      hge_hash_maybe_resize(hash);
    }
}

int
HGE_hash_remove(HGE_hash_t* hash,
		const void* key)
{
  HGE_hash_node_t*  dest;
  HGE_hash_node_t** node;

  node = hge_hash_lookup_node(hash, key);
  if(*node != NULL)
    {
      dest = *node;
      (*node) = dest->next;
      hge_hash_node_free(dest, 
			 hash->key_free_func,
			 hash->value_free_func);
      hash->num_nodes--;

      hge_hash_maybe_resize(hash);

      return HGE_TRUE;
    }
  
  return HGE_FALSE;
}

void
HGE_hash_remove_all(HGE_hash_t* hash)
{
  unsigned int i;

  for(i = 0; i < hash->size; i++)
    {
      hge_hash_nodes_free(hash->nodes[i],
			  hash->key_free_func,
			  hash->value_free_func);
      hash->nodes[i] = NULL;
    }

  hash->num_nodes = 0;
  hge_hash_resize(hash);
}

int
HGE_hash_steal(HGE_hash_t* hash,
	       const void* key)
{
  HGE_hash_node_t*  dest;
  HGE_hash_node_t** node;

  node = hge_hash_lookup_node(hash, key);
  if(*node != NULL) 
    {
      dest = *node;
      (*node) = dest->next;
      hge_hash_node_free(dest, NULL, NULL);
      hash->num_nodes--;
      
      hge_hash_maybe_resize(hash);

      return HGE_TRUE;
    }

  return HGE_FALSE;
}

void
HGE_hash_steal_all(HGE_hash_t* hash)
{
  unsigned int i;

  for(i = 0; i < hash->size; i++)
    {
      hge_hash_nodes_free(hash->nodes[i], NULL, NULL);
      hash->nodes[i] = NULL;
    }
  
  hash->num_nodes = 0;
  hge_hash_resize(hash);
}

void
HGE_hash_foreach0(HGE_hash_t* hash, HGE_hfunc0_t func)
{
  unsigned int i;
  HGE_hash_node_t* node;
  HGE_hash_node_t* next;

  for(i = 0; i < hash->size; i++)
    {
      node = hash->nodes[i];
      while(node != NULL)
	{
	  next = node->next;
	  func(node->key, node->value);
	  node = next;
	}
    }
}

void
HGE_hash_foreach1(HGE_hash_t*  hash,
		  HGE_hfunc1_t func,
		  void*        user_data)
{
  unsigned int i;
  HGE_hash_node_t* node;
  HGE_hash_node_t* next;

  for(i = 0; i < hash->size; i++)
    {
      node = hash->nodes[i];
      while(node != NULL)
	{
	  next = node->next;
	  func(node->key, node->value, user_data);
	  node = next;
	}
    }
}

HGE_uint_t
HGE_hash_foreach0_remove(HGE_hash_t* hash, HGE_hffunc0_t func)
{
  return 0;
}

HGE_uint_t
HGE_hash_foreach1_remove(HGE_hash_t* hash, HGE_hffunc1_t func, void* userdata)
{
  return 0;
}
