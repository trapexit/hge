#include <stdio.h>
#include "HGE.h"

int
main(int argc, char** argv)
{
  int xres,yres;
  HGE_sprite_t* spr;
  HGE_cine_t* cine;

  HGE_cmdline_parse(argc, argv);
  HGE_initialize_video();
  HGE_initialize_timer();

  HGE_config_get_resolution(&xres, &yres);

  HGE_enable_alpha_channel();

  spr = HGE_new(HGE_sprite);
  spr->image = HGE_image_new_from_file(argv[1], NULL);

  cine = HGE_new(HGE_cine);
  HGE_postoffice_set_fh(cine, HGE_cine);

  HGE_cine_add_sprite(cine, spr);
  HGE_postoffice_send(HGE_time_geti()+8000, NULL, cine, HGE_CINE_DONE, NULL);

  {
    HGE_argv_t* argv = HGE_argv_alloc();

    HGE_argv_set_length(argv, 3);

    HGE_argv_set_float(argv, 0, 100.0);
    HGE_argv_set_float(argv, 1, 100.0);
    HGE_argv_set_float(argv, 2, 5.0);

    HGE_postoffice_set_fh(spr, HGE_sprite);
    HGE_postoffice_send(HGE_time_geti()+2000, NULL, spr, HGE_SPR_GOTO_XY, argv);
  }

  HGE_cine_run(cine);

  HGE_unref(cine);
  HGE_unref(spr->image);
  HGE_unref(spr);

  HGE_quit();

  return 0;
}

