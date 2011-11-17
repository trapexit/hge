#ifndef _HGE_FONT_H_
#define _HGE_FONT_H_

#include "HGE_image.h"
#include "HGE_transform.h"

#define HGE_FONT_CENTER 1 << 2

typedef struct HGE_font_t HGE_font_t;
struct HGE_font_t
{
  HGE_image_t* chars;
  char         start;
  char         end;
};

extern HGE_typeinfo_t HGE_font[];

HGE_error_t HGE_font_init(void* ptr);
void        HGE_font_clean(void* ptr);
void*       HGE_font_alloc(void);
void        HGE_font_free(void* ptr);

void* HGE_font_alloc_from(const char* path, char start, char end, int w, int h);
void* HGE_font_alloc_from_hge(const char* path, char start);

void  HGE_font_resize(HGE_font_t* font, char start, char end);
int   HGE_font_load_hge(HGE_font_t* font, SDL_Surface* s, char start);
int   HGE_font_load_sfont(HGE_font_t* font, SDL_Surface* s);
int   HGE_font_load2(HGE_font_t* font, SDL_Surface* s, char start, char end);
int   HGE_font_load(HGE_font_t* font, const char* path, char start, char end, int w, int h);
void  HGE_font_set_char(HGE_font_t* font, char c, HGE_image_t* img);
void  HGE_font_draw(HGE_font_t* font, float x, float y, char* str);
void  HGE_font_draw_transformed(HGE_font_t* font, float x, float y, char* str, HGE_transform_t* t);
void  HGE_font_printf(HGE_font_t* font, float x, float y, char* str, ...);
float HGE_font_str_length(HGE_font_t* font, char* str);
float HGE_font_str_height(HGE_font_t* font, char* str);

#endif /* _HGE_FONT_H_ */
