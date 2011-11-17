#include "HGE_transform.h"

void
HGE_transform_init(HGE_transform_t* tf)
{
  tf->scale    = 1.0;
  tf->rotation = 0.0;
  tf->flags    = 0;
}

HGE_transform_t*
HGE_transform_alloc(void)
{
  HGE_transform_t* tf;

  tf = HGE_alloc(HGE_transform_t);
  if(tf != NULL)
    HGE_transform_init(tf);

  return tf;
}

void
HGE_transform_free(HGE_transform_t* tf)
{
  HGE_free(tf);
}

