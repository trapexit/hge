#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_time.h"
#include "HGE_loop.h"

HGE_typeinfo_t HGE_loop[] = {{sizeof(HGE_loop_t),
			     HGE_loop_init,
			     HGE_loop_clean,
			     NULL,
			     NULL}};

HGE_error_t
HGE_loop_init(void* ptr)
{
  HGE_loop_t* loop = ptr;

  loop->done       = HGE_FALSE;
  loop->dt         = 0.0f;
  loop->max_dt     = 0.2;
  loop->replace_dt = 0.01f;
  loop->fps        = 0;
  loop->t0         = 0;
  loop->fps_time   = 0;
  loop->fps_count  = 0;
  loop->frame_func = NULL;
  loop->data       = NULL;

  return HGE_SUCCESS;
}

void
HGE_loop_clean(void* ptr)
{
}

void*
HGE_loop_alloc(void)
{
  void* ptr;

  if((ptr = HGE_alloc(HGE_loop_t)) != NULL)
    {
      if(HGE_loop_init(ptr) != HGE_SUCCESS)
	{
	  HGE_free(ptr);
	  ptr = NULL;
	}
    }

  return ptr;
}

void
HGE_loop_free(void* ptr)
{
  HGE_loop_clean(ptr);
  HGE_free(ptr);
}

void
HGE_loop_run(HGE_loop_t* loop)
{
  if(loop->frame_func == NULL)
    return;

  while(!loop->done)
    {
      loop->dt = ((float)(HGE_time_geti() - loop->t0) * 0.001f);
      if(loop->dt > loop->max_dt)
	loop->dt = loop->replace_dt;

      loop->t0 = HGE_time_geti();
      if((loop->t0 - loop->fps_time) < 1000)
	loop->fps_count++;
      else
	{
	  loop->fps       = loop->fps_count;
	  loop->fps_count = 0;
	  loop->fps_time  = loop->t0;
	}

      loop->frame_func(loop);
    }
}

void
HGE_loop_run_once(HGE_loop_t* loop)
{
  if(!loop->done && loop->frame_func != NULL)
    {
      loop->dt = ((float)(HGE_time_geti() - loop->t0) * 0.001f);
      if(loop->dt > loop->max_dt)
	loop->dt = loop->replace_dt;
      
      loop->t0 = HGE_time_geti();
      if((loop->t0 - loop->fps_time) < 1000)
	loop->fps_count++;
      else
	{
	  loop->fps       = loop->fps_count;
	  loop->fps_count = 0;
	  loop->fps_time  = loop->t0;
	}
      
      loop->frame_func(loop);
    }
}
