#include "HGE.h"

int
main(int argc, char** argv)
{
  HGE_argv_t* arg;
  Uint8 u8;
  float f;
  char* str = "blah";

  u8 = 10;
  f  = 10.0;
  arg = HGE_argv_alloc();
  HGE_argv_set_from_string(arg, "bfp", u8, f, str);
  u8 = 0;
  f  = 0.0;
  str = NULL;

  HGE_argv_get_uint8(arg, 0, &u8);
  HGE_argv_get_float(arg, 1, &f);
  HGE_argv_get_voidp(arg, 2, (void*)&str);

  printf("%d %f %s\n", u8, f, str);

  HGE_argv_free(arg);

  return 0;
}
