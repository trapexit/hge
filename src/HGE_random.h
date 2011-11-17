#ifndef _HGE_RANDOM_H_
#define _HGE_RANDOM_H_

#include "SDL.h"

void  HGE_random_seed(unsigned int seed);
int   HGE_random_int(int low, int high);
float HGE_random_float(float low, float high);

#endif /* _HGE_RANDOM_H_ */
