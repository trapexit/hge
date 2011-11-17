#!/bin/sh

cat > HGElua.h<<EOF
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lua_gl.h"
#include "lua_sdl.h"
#include "lua_hge.h"
EOF