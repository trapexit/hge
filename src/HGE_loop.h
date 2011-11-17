#ifndef _HGE_LOOP_H_
#define _HGE_LOOP_H_

#include "HGE_basetypes.h"

typedef struct HGE_loop_t HGE_loop_t;
struct HGE_loop_t
{
  int   done;
  float dt;
  float max_dt;
  float replace_dt;
  unsigned int t0;
  unsigned int fps_time;
  unsigned int fps_count;
  unsigned int fps;
  void (*frame_func)(HGE_loop_t* app);
  void* data;
};

extern HGE_typeinfo_t HGE_loop[];

#define HGE_loop_get_dt(app) ((app)->dt)
#define HGE_loop_get_fps(app) ((app)->fps)
#define HGE_loop_set_framefunc(app, func) ((app)->frame_func = func)
#define HGE_loop_get_framefunc(app) ((app)->frame_func)
#define HGE_loop_set_data(app, d) ((app)->data = d)
#define HGE_loop_get_data(app) ((app)->data)
#define HGE_loop_set_max_dt(app, x) ((app)->max_dt = x)
#define HGE_loop_get_max_dt(app) ((app)->max_dt)
#define HGE_loop_set_replace_dt(app, x) ((app)->replace_dt = x)
#define HGE_loop_get_replace_dt(app) ((app)->replace_dt)

HGE_error_t HGE_loop_init(void* ptr);
void        HGE_loop_clean(void* ptr);
void*       HGE_loop_alloc(void);
void        HGE_loop_free(void* ptr);

void HGE_loop_run(HGE_loop_t* app);
void HGE_loop_run_once(HGE_loop_t* app);

#endif /* _HGE_LOOP_H_ */
