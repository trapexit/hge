#ifndef _HGE_MUTEX_H_
#define _HGE_MUTEX_H_

#include "SDL.h"
#include "HGE_typeinfo.h"

/*
  Unlock returns 0 or -1 on error.
 */
#define HGE_mutex_lock(m) SDL_LockMutex(m)
#define HGE_mutex_unlock(m) SDL_UnlockMutex(m)

void* HGE_mutex_alloc(void);
void  HGE_mutex_free(void* ptr);

#endif /* _HGE_MUTEX_H_ */
