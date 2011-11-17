#ifndef _HGE_IMAGE_H_
#define _HGE_IMAGE_H_

#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_types.h"
#include "HGE_transform.h"

extern HGE_typeinfo_t HGE_image[];

typedef struct HGE_image_t HGE_image_t;
struct HGE_image_t
{
  GLuint  texture; /* OpenGL texture name              */
  GLfloat w;       /* draw width in pixels             */
  GLfloat h;       /* draw height in pixels            */
  GLfloat btwp;    /* begin texture width percentage   */
  GLfloat bthp;    /* begin texture height percentage  */
  GLfloat etwp;    /* end texture width percentage     */
  GLfloat ethp;    /* end texture height percentage    */
};

#define HGE_IMAGE_INIT {0,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f}
#define HGE_image_set_texture(img,tex) {((HGE_image_t*)(img))->texture = tex;}
#define HGE_image_set_wh(img,w,h) {((HGE_image_t*)(img))->w = (GLfloat)w; ((HGE_image_t*)(img))->h = (GLfloat)h;}
#define HGE_image_set_texture_coord(IMG,BTWP,BTHP,ETWP,ETHP) \
  ({ ((HGE_image_t*)(IMG))->btwp = (GLfloat)BTWP;	     \
    ((HGE_image_t*)(IMG))->bthp = (GLfloat)BTHP;	     \
    ((HGE_image_t*)(IMG))->etwp = (GLfloat)ETWP;	     \
    ((HGE_image_t*)(IMG))->ethp = (GLfloat)ETHP; })
#define HGE_image_set_texture_coordv(IMG,C) \
  ({ ((HGE_image_t*)(IMG))->btwp = ((GLfloat*)C)[0];	\
    ((HGE_image_t*)(IMG))->bthp = ((GLfloat*)C)[1];	\
    ((HGE_image_t*)(IMG))->etwp = ((GLfloat*)C)[2];	\
    ((HGE_image_t*)(IMG))->ethp = ((GLfloat*)C)[3]; })
    

HGE_error_t HGE_image_init(void*);
void        HGE_image_clean(void*);
void*       HGE_image_alloc(void);
void        HGE_image_free(void*);

void* HGE_image_alloc_from_file(const char* path, SDL_Rect* rect);
void* HGE_image_alloc_from_surface(SDL_Surface* surface, SDL_Rect* rect);
void* HGE_image_new_from_file(const char* path, SDL_Rect* rect);
void* HGE_image_new_from_surface(SDL_Surface* surface, SDL_Rect* rect);

HGE_error_t HGE_image_make_from_file(HGE_image_t* img,
                                     const char* path,
                                     SDL_Rect* rect);
HGE_error_t HGE_image_make_from_surface(HGE_image_t* img,
                                        SDL_Surface* surface,
                                        SDL_Rect* rect);
HGE_error_t HGE_image_make_from_surface2(HGE_image_t* img,
                                         SDL_Surface* surface,
                                         SDL_Rect* rect,
                                         GLuint format);
void  HGE_image_make_tiled(HGE_image_t* img, GLfloat w, GLfloat h);

void  HGE_image_draw(HGE_image_t* i, GLfloat x, GLfloat y);
void  HGE_image_draw_scaled(HGE_image_t* i, GLfloat x, GLfloat y, GLfloat scale);
void  HGE_image_draw_rotated(HGE_image_t* i, GLfloat x, GLfloat y, GLfloat deg);
void  HGE_image_draw_transformed(HGE_image_t* i, GLfloat x, GLfloat y, HGE_transform_t* t);


#endif
