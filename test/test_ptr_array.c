#include "HGE.h"

int
main(int argc, char** argv)
{
  int i;
  HGE_parray_t* parray;

  parray = HGE_parray_alloc();

  HGE_parray_add(parray, (void*)0xdeadbeaf);
  HGE_parray_add(parray, (void*)0x0f0f0f0f);

  for(i = 0; i < parray->length; i++)
    printf("%d : %p\n", i, HGE_parray_index(parray, i));

  return 0;
}
