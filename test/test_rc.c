#include "HGE.h"

int
main(int argc, char** argv)
{
  void* font;

  font = HGE_new(HGE_font);

  HGE_ref(font);

  printf("%d\n", HGE_refcount(font));
  HGE_unref(font);
  printf("%d\n", HGE_refcount(font));
  HGE_unref(font);
  printf("%d\n", HGE_refcount(font));

  return 0;
}
