#include <stdio.h>
#include <stddef.h>
#include "HGE.h"
#include "HGElua.h"

int
main(int argc, char** argv)
{
  lua_State* L;

  L = lua_open();

  luaopen_base(L);
  luaopen_hge(L);
  luaopen_gl(L);
  luaopen_sdl(L);

  luaL_loadfile(L, argv[1]);
  lua_pcall(L,0,0,0);

  return 0;
}
