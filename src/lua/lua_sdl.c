#include "lua.h"
#include "lauxlib.h"
#include "lua_hge.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_image.h"

static int
lua_sdl_delay(lua_State* L)
{
  SDL_Delay((Uint32)lua_tonumber(L, 1));
  return 0;
}

static int
lua_sdl_surface_set_w(lua_State* L)
{
  SDL_Surface* this = (SDL_Surface*)lua_touserdata(L, 1);
  if(this != NULL)
    this->w = (int)lua_tonumber(L, 2);
  return 0;
}

static int
lua_sdl_surface_get_w(lua_State* L)
{
  SDL_Surface* this = (SDL_Surface*)lua_touserdata(L, 1);
  if(this != NULL)
    lua_pushnumber(L,(lua_Number)this->w);
  return 1;
}

static int
lua_sdl_getvideosurface(lua_State* L)
{
  lua_pushlightuserdata(L, SDL_GetVideoSurface());

  return 1;
}

static int
lua_sdl_img_load(lua_State* L)
{
  SDL_Surface* surface = IMG_Load(lua_tostring(L, 1));

  lua_pushlightuserdata(L, surface);

  return 1;
}

static const luaL_reg sdllib[] =
  {
    {"Delay", lua_sdl_delay},
    {"GetVideoSurface", lua_sdl_getvideosurface},
    {"IMG_Load", lua_sdl_img_load},
    {NULL, NULL}
  };

int
luaopen_sdl(lua_State* L)
{
  luaL_openlib(L, "SDL", sdllib, 0);

  return 1;
}
