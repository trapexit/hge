#include "HGE.h"

int
main(int argc, char** argv)
{
  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_quit();

  return 0;
}
