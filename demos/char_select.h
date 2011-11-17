#ifndef _NN_CHAR_SELECT_H_
#define _NN_CHAR_SELECT_H_

enum
  {
    ZIPPY,
    GUTRIP,
    HALFWIT,
    BOGEY,
    KLUBBOR,
    KEWPUD,
    P1_ARROW,
    P2_ARROW,
    BACKGROUND,
    FONT
  };

typedef struct NN_cs_screen_t NN_cs_screen_t;
struct NN_cs_screen_t
{
  HGE_loop_t* loop;
  HGE_image_t* bg;
  HGE_font_t* font;
  HGE_sprite_t* arrow[2];
  HGE_image_t* charbust[6];
  int p1;
  int p2;
};

extern HGE_typeinfo_t NN_cs_screen[];

HGE_error_t NN_cs_screen_init(void* ptr);
void        NN_cs_screen_clean(void* ptr);

#endif /* _NN_CHAR_SELECT_H_ */
