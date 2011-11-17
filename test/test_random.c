#include "HGE.h"

int
main(int argc, char** argv)
{
  int i;

  //HGE_initialize_video();
  //HGE_initialize_timer();

  HGE_random_seed(SDL_GetTicks());

  for(i = 0; i < 100000; i++)
    printf("%d ", HGE_random_int(0, 10));

/*   for(i = 0; i < 10; i++) */
/*     printf("%d: %d\n", i, a[i]); */

  //HGE_quit();

  return 0;
}
