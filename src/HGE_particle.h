#ifndef _HGE_PARTICLE_H_
#define _HGE_PARTICLE_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_types.h"
#include "HGE_argv.h"

enum 
  {
    HGE_EMITTER_POINTS,
    HGE_EMITTER_POINT_SPRITES,
    HGE_EMITTER_LINES,
    HGE_EMITTER_QUADS,
    HGE_EMITTER_LAST
  };

typedef struct HGE_particle_t HGE_particle_t;
struct HGE_particle_t
{
  float x,y;
  float vx,vy;
  float ax,ay;
  GLubyte r,g,b,a;
  float age;
  float death_age;
};

typedef struct HGE_emitter_t HGE_emitter_t;
struct HGE_emitter_t
{
  HGE_particle_t* particles;
  unsigned int    count;
  unsigned int    max;

  int   paused;
  float age;
  float size;
  float pps;
  float emit_extra;
  float direction,spread;
  float age_min,age_max;
  float x_min,x_max;
  float y_min,y_max;
  float vx_min,vx_max;
  float vy_min,vy_max;
  float ax_min,ax_max;
  float ay_min,ay_max;
  GLubyte r_s,g_s,b_s,a_s;
  GLubyte r_e,g_e,b_e,a_e;
  GLenum blend_sfactor, blend_dfactor;
  
  void (*update)(HGE_emitter_t* e, float dt);
  void (*add_particles)(HGE_emitter_t* e, float dt);
  void (*draw)(HGE_emitter_t* e);
  void (*free)(HGE_emitter_t* e);
  void* extra;
};

extern HGE_typeinfo_t HGE_emitter[];

HGE_error_t HGE_emitter_init(void* ptr);
void        HGE_emitter_clean(void* ptr);
void*       HGE_emitter_alloc(void);
void        HGE_emitter_free(void* ptr);

int   HGE_emitter_set_type(HGE_emitter_t* e, unsigned int type);
void* HGE_emitter_alloc_type(unsigned int type);
void  HGE_emitter_set_size(HGE_emitter_t* e, unsigned int size);
void  HGE_emitter_draw(HGE_emitter_t* e, float x, float y);
void  HGE_emitter_update(HGE_emitter_t* e, float dt);

void HGE_emitter_update_generic(HGE_emitter_t* e, float dt);
void HGE_emitter_add_particles_generic(HGE_emitter_t* e, float dt);
void HGE_emitter_draw_points(HGE_emitter_t* e);
void HGE_emitter_draw_point_sprites(HGE_emitter_t* e);
void HGE_emitter_draw_lines(HGE_emitter_t* e);
void HGE_emitter_draw_quads(HGE_emitter_t* e);

#endif /* _HGE_PARTICLE_H_ */
