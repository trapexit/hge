#ifndef _HGE_DISTORTED_H_
#define _HGE_DISTORTED_H_

#include "HGE_basetypes.h"
#include "HGE_types.h"
#include "HGE_image.h"

enum
  {
    HGE_DISTORT_POINT_ABS,
    HGE_DISTORT_POINT_REL,
    HGE_DISTORT_TOPLEFT,
    HGE_DISTORT_CENTER
  };

typedef struct HGE_distorted_t HGE_distorted_t;
struct HGE_distorted_t
{
  HGE_image_t*    img;
  HGE_uint_t      grid_w;
  HGE_uint_t      grid_h;
  HGE_spfloat_t   tex_w;
  HGE_spfloat_t   tex_h;
  HGE_c4ub_v2f_t* points;
  HGE_uint_t      flags;
};

extern HGE_typeinfo_t HGE_distorted[];

HGE_error_t HGE_distorted_init(void* ptr);
void        HGE_distorted_clean(void* ptr);
void*       HGE_distorted_alloc(void);
void        HGE_distorted_free(void* ptr);

void HGE_distorted_set_image(HGE_distorted_t* d, HGE_image_t* img);
HGE_image_t* HGE_distorted_get_image(HGE_distorted_t* d);
void HGE_distorted_set_mesh_size(HGE_distorted_t* d, unsigned int w, unsigned int h);
void HGE_distorted_get_mesh_size(HGE_distorted_t* d, unsigned int* w, unsigned int* h);
void HGE_distorted_reset_mesh(HGE_distorted_t* d);
void HGE_distorted_set_color(HGE_distorted_t* d, int x, int y, GLubyte r, GLubyte g, GLubyte b, GLubyte a);
void HGE_distorted_get_color(HGE_distorted_t* d, int x, int y, GLubyte* r, GLubyte* g, GLubyte* b, GLubyte* a);
void HGE_distorted_set_displacement(HGE_distorted_t* d, int x, int y, GLfloat dx, GLfloat dy, int ref);
void HGE_distorted_get_displacement(HGE_distorted_t* d, int x, int y, GLfloat* dx, GLfloat* dy,  int ref);
void HGE_distorted_draw_mesh(HGE_distorted_t*,HGE_spfloat_t,HGE_spfloat_t);
void HGE_distorted_draw(HGE_distorted_t* d, GLfloat x, GLfloat y);
void HGE_distorted_draw_transformed(HGE_distorted_t* d, GLfloat x, GLfloat y, HGE_transform_t* t);

#endif  /* _HGE_DISTORTED_H_ */
