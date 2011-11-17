#include "HGE.h"

int
main(int argc, char** argv)
{
  if(HGE_initialize_video())
    HGE_gl_print_info();

  HGE_quit();

  return 0;
}
