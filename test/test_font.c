#include "HGE.h"

int
main(int argc, char** argv)
{
  HGE_transform_t t;
  HGE_font_t font;

  HGE_cmdline_parse(argc, argv);

  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_enable_alpha_channel();
  HGE_color_default_blend();

  HGE_font_init(&font);

  SDL_Surface* b;
  b = IMG_Load("24P_Arial_NeonYellow.png");
  //  b = HGE_surface_make_rgba8888(b, NULL);

  HGE_font_load_sfont(&font, b);

  SDL_FreeSurface(b);

  //  HGE_font_load(&font, argv[1], ' ', '~', atoi(argv[2]), atoi(argv[3]));

  HGE_font_draw(&font, 100.0f, 100.0f, " !\"#$%&'()*+,-./");
  HGE_font_draw(&font, 100.0f, 120.0f, "0123456789:;<=>?");
  
  HGE_transform_init(&t);
  t.rotation = 10;
  t.scale = 2;
  HGE_texture_set_filter(font.chars[0].texture, GL_LINEAR);
  HGE_font_draw_transformed(&font, 100.0f, 140.0f, "Antonio SJ Musumeci\nScale: 2.0\nRotation: 10 Degrees\nGL_LINEAR Filter", &t);

  HGE_texture_set_filter(font.chars[0].texture, GL_NEAREST);
  HGE_font_printf(&font, 150, 50, "Font size: %sx%s\nPrinted with HGE_font_printf", argv[2], argv[3]);

  HGE_color_set3(255, 0, 0);
  HGE_font_draw(&font, 0, 0, "RED");  
  HGE_color_set3(0, 255, 0);
  HGE_font_draw(&font, 0, 20, "GREEN");  
  HGE_color_set3(0, 0, 255);
  HGE_font_draw(&font, 0, 40, "BLUE");  

  t.scale = 1.0;
  t.rotation = 45.0f;
  t.flags |= HGE_FONT_CENTER;
  HGE_font_draw_transformed(&font, 500.0f, 40.0f, "ASDF\nasdfasdf\nCRAP", &t);

  HGE_swapbuffers();
  HGE_take_screenshot("screenshot.bmp");

  SDL_Delay(5000);

  HGE_quit();

  return 0;
}
