#include "lua.h"
#include "lauxlib.h"
#include "lua_gl.h"
#include "GL/gl.h"
#include "HGE_gl_funcs.h"

static int
lua_HGE_glBegin(lua_State* L)
{
  HGE_glBegin((GLenum)lua_touserdata(L, 1));
  return 0;
}

static int
lua_HGE_glEnd(lua_State* L)
{
  HGE_glEnd();
  return 0;
}

static int
lua_HGE_glVertex2f(lua_State* L)
{
  HGE_glVertex2f((GLfloat)lua_tonumber(L,1),
		 (GLfloat)lua_tonumber(L,2));
  return 0;
}

static int
lua_HGE_glTexCoord2f(lua_State* L)
{
  HGE_glTexCoord2f((GLfloat)lua_tonumber(L,1),
		   (GLfloat)lua_tonumber(L, 2));
  return 0;
}

static int
lua_HGE_glBindTexture(lua_State* L)
{
  HGE_glBindTexture((GLenum)lua_touserdata(L,1),
		    (GLuint)lua_tonumber(L,2));
  return 0;
}

static const luaL_reg gllib[] =
  {
    {"Begin",       lua_HGE_glBegin},
    {"End",         lua_HGE_glEnd},
    {"Vertex2f",    lua_HGE_glVertex2f},
    {"TexCoord2f",  lua_HGE_glTexCoord2f},
    {"BindTexture", lua_HGE_glBindTexture},

    {NULL, NULL}
  };

static struct {char* name; GLenum value;} enum_array[] =
{
  {"POINTS",         GL_POINTS},
  {"LINES",          GL_LINES},
  {"LINE_STRIP",     GL_LINE_STRIP},
  {"LINE_LOOP",      GL_LINE_LOOP},
  {"TRIANGLES",      GL_TRIANGLES},
  {"TRIANGLE_STRIP", GL_TRIANGLE_STRIP},
  {"TRIANGLE_FAN",   GL_TRIANGLE_FAN},
  {"QUADS",          GL_QUADS},
  {"QUAD_STRIP",     GL_QUAD_STRIP},
  {"POLYGON",        GL_POLYGON},
  {"TEXTURE_1D",     GL_TEXTURE_1D},
  {"TEXTURE_2D",     GL_TEXTURE_2D}
};

int
luaopen_gl(lua_State* L)
{
  int i;
  luaL_openlib(L, "gl", gllib, 0);

  for(i = 0; i < (sizeof(enum_array) / sizeof(enum_array[0])); i++)
    {
      lua_pushstring(L, enum_array[i].name);
      lua_pushlightuserdata(L, (void*)enum_array[i].value);
      lua_settable(L, -3);      
    }

  return 1;
}
