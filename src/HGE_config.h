#ifndef _HGE_CONFIG_H_
#define _HGE_CONFIG_H_

#include "SDL.h"
#include "SDL_opengl.h"

enum
  {
    NON_POWER_OF_2_EXT      = 0x01,
    TEXTURE_COMPRESSION_EXT = 0x02,
    SET_SYNC_TO_VBLANK      = 0x04
  };

typedef struct HGE_config_t HGE_config_t;
struct HGE_config_t
{
  Uint32 videoflags;
  int    doublebuffer;
  int    red_size;
  int    green_size;
  int    blue_size;
  int    alpha_size;
  int    audio;
  int    audio_channels;
  int    audio_buffer_size;
  int    x_res;
  int    y_res;
  Uint32 gl_flags;
  GLuint texture_format;
  SDL_Surface* screen;
};

void HGE_config_copy_config(HGE_config_t* c);
HGE_config_t* HGE_config_get_config(void);
void HGE_config_set_config(HGE_config_t* c);
void HGE_config_set_fullscreen(int value);
int  HGE_config_get_fullscreen(void);
void HGE_config_set_no_frame(int value);
int  HGE_config_get_no_frame(void);
void HGE_config_set_resizable(int value);
int  HGE_config_get_resizable(void);
void HGE_config_set_resolution(int x_res, int y_res);
void HGE_config_get_resolution(int* x_res, int* y_res);
void HGE_config_set_audio(int value);
void HGE_config_set_audio_channels(int value);
void HGE_config_set_audio_buffer_size(int value);
void HGE_config_set_texture_format(GLuint format);
GLuint HGE_config_get_texture_format(void);
void HGE_config_set_doublebuffer(int value);
int  HGE_config_get_double_buffer(void);
void HGE_config_set_rgba(int r, int g, int b, int a);
void HGE_config_get_rgba(int* r, int* g, int* b, int* a);

#endif
