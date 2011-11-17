#include <stdlib.h>
#include "SDL.h"
#include "HGE_random.h"

static unsigned int _offset_ = 0;
static unsigned int _array_[256];

void
HGE_random_seed(unsigned int seed)
{
  int i;

  _array_[0] = seed + SDL_GetTicks();
  for(i = 1; i < (sizeof(_array_) / sizeof(_array_[0])); i++)
    {
      _array_[i] = (SDL_GetTicks() * 
		    ((_array_[sizeof(_array_)/sizeof(_array_[0]) - i] +
		    ((unsigned int)HGE_random_seed ^
		     _array_[i-1])) & (unsigned int)_array_));
    }

}

int
HGE_random_int(int low, int high)
{
  unsigned int* v;

  if((++_offset_) >= (sizeof(_array_) - sizeof(_array_[0])))
    _offset_ = 0;

  v = (unsigned int*)&(((char*)_array_)[_offset_]);

  *v = ((0xdead * *v) + (unsigned int)v);

  return ((*v) % (high - low)) + low;
}

float
HGE_random_float(float low, float high)
{
  unsigned int* v;

  if((++_offset_) > (sizeof(_array_) - sizeof(_array_[0])))
    _offset_ = 0;

  v = (unsigned int*)&(((char*)_array_)[_offset_]);

  *v = ((0xbeef * *v) + (unsigned int)v);

  return (((float)*v / 4294967295.0f) * (high - low)) + low;
}
