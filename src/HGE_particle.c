#include <math.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_types.h"
#include "HGE_mem.h"
#include "HGE_gl_funcs.h"
#include "HGE_color.h"
#include "HGE_particle.h"
#include "HGE_random.h"

HGE_typeinfo_t HGE_emitter[] = {{sizeof(HGE_emitter_t),
				 HGE_emitter_init,
				 HGE_emitter_clean,
				 NULL,
				 NULL}};

typedef void (*HGE_emitter_drawer_t)(HGE_emitter_t*);
typedef struct HGE_emitter_data_t HGE_emitter_data_t;
struct HGE_emitter_data_t
{
  HGE_emitter_drawer_t drawer;
  GLenum blend_sfactor;
  GLenum blend_dfactor;
};

const HGE_emitter_data_t emitter_data[] =
  {
    {HGE_emitter_draw_points,        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
    {HGE_emitter_draw_point_sprites, GL_SRC_ALPHA, GL_ONE},
    {HGE_emitter_draw_lines,         GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA},
    {HGE_emitter_draw_quads,         GL_SRC_ALPHA, GL_ONE}
  };

HGE_error_t
HGE_emitter_init(void* ptr)
{
  HGE_emitter_t* e = ptr;

  e->paused     = HGE_FALSE;
  e->count      = 0;
  e->max        = 0;
  e->particles  = NULL;

  e->emit_extra = 0.0f;
  e->pps        = 1.0f;
  e->age_min    = 0.0f;
  e->age_max    = 1.0f;
  e->direction  = 0.0f;
  e->spread     = 0.0f;
  e->x_min      = 0.0f;
  e->x_max      = 0.0f;
  e->y_min      = 0.0f;
  e->y_max      = 0.0f;
  e->vx_min     = 0.0f;
  e->vx_max     = 0.0f;
  e->vy_min     = 0.0f;
  e->vy_max     = 0.0f;
  e->ax_min     = 0.0f;
  e->ax_max     = 0.0f;
  e->ay_min     = 0.0f;
  e->ay_max     = 0.0f;
  e->r_s = e->g_s = e->b_s = e->a_s = 255;
  e->r_e = e->g_e = e->b_e = e->a_e = 255;
  e->blend_sfactor = GL_SRC_ALPHA;
  e->blend_dfactor = GL_ONE_MINUS_SRC_ALPHA;

  e->update = HGE_emitter_update_generic;
  e->add_particles = HGE_emitter_add_particles_generic;
  e->draw = HGE_emitter_draw_points;
  e->free = NULL;

  return HGE_SUCCESS;
}

void
HGE_emitter_clean(void* ptr)
{
  HGE_emitter_t* e = ptr;

  if(e != NULL)
    {
      if(e->free != NULL)
	e->free(e);
      HGE_free(e->particles);
    } 
}

void*
HGE_emitter_alloc(void)
{
  void* ptr;
  
  if((ptr = HGE_alloc(HGE_emitter_t)) != NULL)
    HGE_emitter_init(ptr);

  return ptr;
}

void
HGE_emitter_free(void* ptr)
{
  HGE_emitter_clean(ptr);
  HGE_free(ptr);
}

int
HGE_emitter_set_type(HGE_emitter_t* e, unsigned int type)
{
  if(type >= HGE_EMITTER_LAST)
    return -1;

  e->draw          = emitter_data[type].drawer;
  e->blend_sfactor = emitter_data[type].blend_sfactor;
  e->blend_dfactor = emitter_data[type].blend_dfactor;

  return 0;
}

void*
HGE_emitter_alloc_type(unsigned int type)
{
  HGE_emitter_t* e;

  e = HGE_emitter_alloc();
  if(e != NULL)
    {
      HGE_emitter_set_type(e, type);
    }

  return e;
}

void
HGE_emitter_set_size(HGE_emitter_t* e, unsigned int size)
{
  void* tmp;

  tmp = HGE_realloc(e->particles, HGE_particle_t, size);
  if(tmp != NULL)
    {
      e->max = size;
      e->particles = tmp;
    }
}

void
HGE_emitter_draw(HGE_emitter_t* e, float x, float y)
{
  HGE_glPushMatrix();
  HGE_glTranslatef(x, y, 0.0);

  e->draw(e);

  HGE_glPopMatrix();
}

void
HGE_emitter_update(HGE_emitter_t* e, float dt)
{
  e->update(e, dt);
  if(!e->paused)
    e->add_particles(e, dt);
  e->age += dt;
}

void
HGE_emitter_update_generic(HGE_emitter_t* e, float dt)
{
  unsigned int i = 0;
  float age_ratio;
  HGE_particle_t* p = e->particles;

  while(i < e->count)
    {
      p->age += dt;
      if(p->age > p->death_age)
	{
	  *p = e->particles[--e->count];
	  continue;
	}

      p->x  += ((p->vx * dt) + (p->ax * 0.5 * dt * dt));
      p->y  += ((p->vy * dt) + (p->ay * 0.5 * dt * dt));
      p->vx += (p->ax * dt);
      p->vy += (p->ay * dt);

      age_ratio = (p->age / p->death_age);
      p->r = (GLubyte)(e->r_s + (age_ratio * (e->r_e - e->r_s)));
      p->g = (GLubyte)(e->g_s + (age_ratio * (e->g_e - e->g_s)));
      p->b = (GLubyte)(e->b_s + (age_ratio * (e->b_e - e->b_s)));
      p->a = (GLubyte)(e->a_s + (age_ratio * (e->a_e - e->a_s)));

      p++;
      i++;
    }
}

void
HGE_emitter_add_particles_generic(HGE_emitter_t* e, float dt)
{
  float dir,dirx,diry;  
  float ntc, age_ratio;
  float odt = 0, spp;
  HGE_particle_t* p = &e->particles[e->count];

  spp = 1.0f / e->pps;
  ntc = ((e->pps * dt) + e->emit_extra);
  while((ntc >= 1.0f) && (e->count < e->max))
    {
      p->age = odt;
      p->death_age = HGE_random_float(e->age_min, e->age_max);
      dir  = HGE_random_float(e->direction - e->spread, e->direction + e->spread);
      dirx = cosf(dir * 0.017453293f); /* (M_PI / 180.0f)) */
      diry = sinf(dir * 0.017453293f); /* (M_PI / 180.0f)) */

      p->ax = HGE_random_float(e->ax_min, e->ax_max);
      p->ay = HGE_random_float(e->ay_min, e->ay_max);

      p->vx = (HGE_random_float(e->vx_min, e->vx_max) * dirx);
      p->vy = (HGE_random_float(e->vy_min, e->vy_max) * diry);
      p->vx += (p->ax * odt);
      p->vy += (p->ay * odt);

      p->x = HGE_random_float(e->x_min, e->x_max);
      p->y = HGE_random_float(e->y_min, e->y_max);
      p->x += ((p->vx * odt) + (p->ax * 0.5f * odt * odt));
      p->y += ((p->vy * odt) + (p->ay * 0.5f * odt * odt));

      age_ratio = (p->age / p->death_age);
      p->r = (GLubyte)(e->r_s + (age_ratio * (e->r_e - e->r_s)));
      p->g = (GLubyte)(e->g_s + (age_ratio * (e->g_e - e->g_s)));
      p->b = (GLubyte)(e->b_s + (age_ratio * (e->b_e - e->b_s)));
      p->a = (GLubyte)(e->a_s + (age_ratio * (e->a_e - e->a_s)));    

      p++;
      e->count++;
      ntc--;
      odt += spp;
    }

  e->emit_extra = ntc;
}

void
HGE_emitter_draw_points(HGE_emitter_t* e)
{
  HGE_glPushAttrib(GL_ENABLE_BIT);
  HGE_glDisable(GL_TEXTURE_2D);
  
  HGE_glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
  HGE_glEnableClientState(GL_VERTEX_ARRAY);
  HGE_glEnableClientState(GL_COLOR_ARRAY);
  HGE_glPointSize(e->size);
  HGE_color_set_blend(e->blend_sfactor, e->blend_dfactor);
  HGE_glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(HGE_particle_t),&e->particles->r);
  HGE_glVertexPointer(2, GL_FLOAT, sizeof(HGE_particle_t), &e->particles->x);
  HGE_glDrawArrays(GL_POINTS, 0, e->count);
  HGE_glPopClientAttrib();
  HGE_glDisableClientState(GL_COLOR_ARRAY);
  HGE_glDisableClientState(GL_VERTEX_ARRAY);

  HGE_glPopAttrib();
}

void
HGE_emitter_draw_point_sprites(HGE_emitter_t* e)
{
  HGE_glPushAttrib(GL_ENABLE_BIT);

  HGE_glEnable(GL_TEXTURE_2D);
  HGE_glBindTexture(GL_TEXTURE_2D, (GLuint)e->extra);
  HGE_glEnable(GL_POINT_SPRITE_ARB);
  HGE_glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);

  HGE_glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
  HGE_glEnableClientState(GL_VERTEX_ARRAY);
  HGE_glEnableClientState(GL_COLOR_ARRAY);
  HGE_glPointSize(e->size);
  HGE_color_set_blend(e->blend_sfactor, e->blend_dfactor);  
  HGE_glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(HGE_particle_t),&e->particles->r);
  HGE_glVertexPointer(2, GL_FLOAT, sizeof(HGE_particle_t), &e->particles->x);
  HGE_glDrawArrays(GL_POINTS, 0, e->count);
  HGE_glPopClientAttrib();
  HGE_glDisableClientState(GL_COLOR_ARRAY);
  HGE_glDisableClientState(GL_VERTEX_ARRAY);
  
  HGE_glPopAttrib();
}

void
HGE_emitter_draw_quads(HGE_emitter_t* e)
{
  int i;
  register float x_size,y_size;
  HGE_particle_t* p = e->particles;

  HGE_glPushAttrib(GL_ENABLE_BIT);
  HGE_glEnable(GL_TEXTURE_2D);

  HGE_color_set_blend(e->blend_sfactor, e->blend_dfactor);
  HGE_glBindTexture(GL_TEXTURE_2D, (GLuint)e->extra);
  HGE_glBegin(GL_QUADS);
  for(i = 0; i < e->count; i++)
    {
      x_size = p->x + e->size;
      y_size = p->y + e->size;
      HGE_glColor4ubv(&p->r);
      HGE_glTexCoord2f(0.0f, 0.0f); HGE_glVertex2f(p->x, p->y);
      HGE_glTexCoord2f(1.0f, 0.0f); HGE_glVertex2f(x_size, p->y);
      HGE_glTexCoord2f(1.0f, 1.0f); HGE_glVertex2f(x_size, y_size);
      HGE_glTexCoord2f(0.0f, 1.0f); HGE_glVertex2f(p->x, y_size);
      p++;
    }
  HGE_glEnd();

  HGE_glPopAttrib();
}

void
HGE_emitter_draw_lines(HGE_emitter_t* e)
{
  int i;
  HGE_particle_t* p = e->particles;

  HGE_glPushAttrib(GL_ENABLE_BIT);

  HGE_glDisable(GL_TEXTURE_2D);
  HGE_glLineWidth(e->size);
  HGE_color_set_blend(e->blend_sfactor, e->blend_dfactor);  
  HGE_glBegin(GL_LINES);
  for(i = 0; i < e->count; i++)
    {
      HGE_glColor4ub(p->r, p->g, p->b, 0);
      HGE_glVertex2f((p->x - p->vx * 0.2), (p->y - p->vy * 0.2));
      HGE_glColor4ubv(&p->r);
      HGE_glVertex2fv(&p->x);
      p++;
    }
  HGE_glEnd();
  
  HGE_glPopAttrib();
}

