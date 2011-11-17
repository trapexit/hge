#ifndef _HGE_IMAP_H_
#define _HGE_IMAP_H_

#include "HGE_types.h"
#include "HGE_image.h"

/* HGE_imap_t                                                            */
/* a group of HGE_image_t's treated like a single image                  */
/* usually used because of of the max texture size limitations in OpenGL */

typedef struct HGE_imap_t HGE_imap_t;
struct HGE_imap_t
{ 
  int iw; /* images wide */
  int ih; /* images high */
  int mw; /* map width in pixels */
  int mh; /* map height in piexels */ 
  HGE_image_t** images;
};

extern HGE_typeinfo_t HGE_imap[];

HGE_error_t HGE_imap_init(void* imap);
void        HGE_imap_clean(void* imap);
void*       HGE_imap_alloc(void);
void        HGE_imap_free(void* imap);

void* HGE_imap_alloc_from_file(const char* path);
void* HGE_imap_alloc_from_surface(SDL_Surface* surface);
void* HGE_imap_new_from_file(const char* path);
void* HGE_imap_new_from_surface(SDL_Surface* surface);
int   HGE_imap_make_from_file(HGE_imap_t* imap, const char* path);
int   HGE_imap_make_from_surface(HGE_imap_t* imap, SDL_Surface* surface);
void  HGE_imap_set_image(HGE_imap_t* imap, HGE_image_t* image, int x, int y);
void* HGE_imap_get_image(HGE_imap_t* imap, int x, int y);
int   HGE_imap_set_wh(HGE_imap_t* imap, int w, int h);
void  HGE_imap_draw(HGE_imap_t* imap, GLint x, GLint y);
void  HGE_imap_draw_rotate(HGE_imap_t* imap, GLint x, GLint y, GLfloat angle);
void  HGE_imap_draw_transform(HGE_imap_t* imap, GLint x, GLint y, HGE_transform_t* t);

#endif
