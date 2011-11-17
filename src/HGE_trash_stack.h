#ifndef _HGE_TRASH_STACK_H_
#define _HGE_TRASH_STACK_H_

typedef struct HGE_trash_stack_t HGE_trash_stack_t;
struct HGE_trash_stack_t
{
  HGE_trash_stack_t* next;
};

static inline void         HGE_trash_stack_push(HGE_trash_stack_t** stack, void* data);
static inline void*        HGE_trash_stack_pop(HGE_trash_stack_t** stack);
static inline void*        HGE_trash_stack_peek(HGE_trash_stack_t** stack);
static inline unsigned int HGE_trash_stack_height(HGE_trash_stack_t** stack);

static inline void
HGE_trash_stack_push(HGE_trash_stack_t** stack, void* data)
{
  register HGE_trash_stack_t* s = (HGE_trash_stack_t*)data;

  s->next = *stack;
  *stack = s;
}

static inline void*
HGE_trash_stack_pop(HGE_trash_stack_t** stack)
{
  register HGE_trash_stack_t* s;

  s = *stack;
  if(s != NULL)
    {
      *stack = s->next;
      s->next = NULL;
    }

  return s;
}

static inline void*
HGE_trash_stack_peek(HGE_trash_stack_t** stack)
{
  return (HGE_trash_stack_t*)(*stack);
}

static inline unsigned int
HGE_trash_stack_height(HGE_trash_stack_t** stack)
{
  register unsigned int i = 0;
  register HGE_trash_stack_t* s;
  
  s = *stack;
  while(s != NULL)
    {
      i++;
      s = s->next;
    }

  return i;
}

#endif
