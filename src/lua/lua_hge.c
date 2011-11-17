#include "lua.h"
#include "lauxlib.h"
#include "lua_hge.h"
#include "HGE.h"

/* HGE_initialize.h */
static int
lua_hge_initialize_video(lua_State* L)
{
  lua_pushboolean(L, HGE_initialize_video());
  return 1;
}

static int
lua_hge_initialize_timer(lua_State* L)
{
  lua_pushboolean(L, HGE_initialize_timer());
  return 1;
}

static int
lua_hge_initialize_audio(lua_State* L)
{
  lua_pushboolean(L, HGE_initialize_audio());
  return 1;
}

static int
lua_hge_quit(lua_State* L)
{
  HGE_quit();
  return 0;
}

/* HGE_texture.h */
static int
lua_hge_texture_make(lua_State* L)
{
  const GLvoid* data     = lua_topointer(L, 1);
  GLuint x               = (GLuint)lua_tonumber(L, 2);
  GLuint y               = (GLuint)lua_tonumber(L, 3);
  GLuint w               = (GLuint)lua_tonumber(L, 4);
  GLuint h               = (GLuint)lua_tonumber(L, 5);
  GLenum format          = (GLenum)lua_tonumber(L, 6);
  GLenum type            = (GLenum)lua_tonumber(L, 7);
  GLint  internal_format = (GLint)lua_tonumber(L, 8);
  
  lua_pushnumber(L, (lua_Number)HGE_texture_make(data, 
						 x, y, w, h, 
						 format, type, 
						 internal_format));
  return 1;
}

/* HGE_surface */
static int
lua_hge_surface_make_texture(lua_State* L)
{
  GLuint t;
  
  t = HGE_surface_make_texture((SDL_Surface*)lua_topointer(L, 1),
			       (SDL_Rect*)lua_topointer(L, 2));
  lua_pushnumber(L, (lua_Number)t);

  return 1;
}

static int
lua_hge_swapbuffers(lua_State* L)
{
  HGE_swapbuffers();
  return 0;
}

static int
lua_hge_image_alloc(lua_State* L)
{
  HGE_image_t* img;

  img = (HGE_image_t*)lua_newuserdata(L, sizeof(HGE_image_t));
  HGE_image_init(img);

  luaL_getmetatable(L, "HGE_IMAGE_MT");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_hge_image_draw(lua_State* L)
{
  HGE_image_t* img = (HGE_image_t*)lua_touserdata(L, 1);

  HGE_image_draw(img, (GLfloat)lua_tonumber(L, 2), (GLfloat)lua_tonumber(L, 3));

  return 0;
}

static int
lua_hge_image_tostring(lua_State* L)
{
  HGE_image_t* img = (HGE_image_t*)lua_touserdata(L, 1);

  lua_pushfstring(L, "w=%f h=%f", img->w, img->h);
  
  return 1;
}

static int
lua_hge_image_get_w(lua_State* L)
{
  HGE_image_t* img = (HGE_image_t*)lua_touserdata(L, 1);

  lua_pushnumber(L, (lua_Number)img->w);

  return 1;
}

static int
lua_hge_image_set_w(lua_State* L)
{
  HGE_image_t* img = (HGE_image_t*)lua_touserdata(L, 1);

  img->w = (GLfloat)lua_tonumber(L, 2);

  return 0;
}

static int
lua_hge_image_get_h(lua_State* L)
{
  HGE_image_t* img = (HGE_image_t*)lua_touserdata(L, 1);

  lua_pushnumber(L, (lua_Number)img->h);

  return 1;
}

static int
lua_hge_image_set_h(lua_State* L)
{
  HGE_image_t* img = (HGE_image_t*)lua_touserdata(L, 1);

  img->h = (GLfloat)lua_tonumber(L, 2);

  return 0;
}

static const luaL_reg hgeimage[] =
  {
    {"__tostring", lua_hge_image_tostring},
    {"get_w",      lua_hge_image_get_w},
    {"set_w",      lua_hge_image_set_w},
    {"get_h",      lua_hge_image_get_h},
    {"set_h",      lua_hge_image_set_h},
    {NULL, NULL}
  };

static const luaL_reg hgelib[] =
  {
    {"initialize_video",          lua_hge_initialize_video},
    {"initialize_timer",          lua_hge_initialize_timer},
    {"initialize_audio",          lua_hge_initialize_audio},
    {"quit",                      lua_hge_quit},
    {"texture_make",              lua_hge_texture_make},
    {"surface_make_texture",      lua_hge_surface_make_texture},
    {"image_alloc",               lua_hge_image_alloc},
    {"image_draw",                lua_hge_image_draw},
    {"swapbuffers",               lua_hge_swapbuffers},
    {NULL, NULL}
  };

int
luaopen_hge(lua_State* L)
{
  luaL_newmetatable(L, "HGE_IMAGE_MT");
  
  lua_pushstring(L, "__index");
  lua_pushvalue(L, -2);
  lua_settable(L, -3);

  luaL_openlib(L, NULL, hgeimage, 0);
  luaL_openlib(L, "HGE", hgelib, 0);

  return 1;
}
