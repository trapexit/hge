#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "HGE_defines.h"
#include "HGE_config.h"
#include "HGE_mem.h"
#include "HGE_math.h"
#include "HGE_gl_funcs.h"
#include "HGE_gl.h"


extern HGE_config_t global_config;

int
HGE_gl_set_vsync(int n)
{
  static int (*set_vsync)(int) = NULL;

  if(set_vsync == NULL)
    {
#ifdef WIN32
      set_vsync = SDL_GL_GetProcAddress("wglSwapIntervalEXT");
#else
      set_vsync = SDL_GL_GetProcAddress("glXSwapIntervalSGI");
#endif      
    }

  if(set_vsync != NULL)
    return set_vsync(n);
  else
    return -1;
}

void
HGE_gl_enable_textures(void)
{
  HGE_glEnable(GL_TEXTURE_2D);
}

void
HGE_gl_disable_textures(void)
{
  HGE_glDisable(GL_TEXTURE_2D);
}

void
HGE_gl_perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
  GLdouble xmin,xmax,ymin,ymax;

  ymax = (zNear * tan(fovy * M_PI / 360.0));
  ymin = -ymax;
  xmin = (ymin * aspect);
  xmax = (ymax * aspect);

  HGE_glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

GLint
HGE_gl_max_texture_size(void)
{
  static GLint max_texture_size = 0;

  if(max_texture_size == 0)
    HGE_glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);
	
  return max_texture_size;
}

GLuint
HGE_gl_safe_texture_size(GLuint size)
{
  GLuint max_texture_size;

  max_texture_size = HGE_gl_max_texture_size();
  if(size > max_texture_size)
    return max_texture_size;

  if(!HGE_gl_NPOT_texture_allowed())
    size = HGE_math_nearest_power2(size);
  
  if(size > max_texture_size)
    return max_texture_size;

  return size;
}

int
HGE_gl_NPOT_texture_allowed(void)
{
  return (global_config.gl_flags & NON_POWER_OF_2_EXT);
}

void
HGE_gl_print_info(void)
{
  char* extlist;
  char* ext;
  HGE_config_t* config;
  const char* (*wglGetExtensionsStringEXT)(void);
  
  config = HGE_config_get_config();
  printf("Vendor: %s\n"
	 "Renderer: %s\n"
	 "Version: %s\n"
	 "BitsPerChannel:\n"
	 " Red:   %d\n"
	 " Green: %d\n"
	 " Blue:  %d\n"
	 " Alpha: %d\n"
	 "DoubleBuffered: %d\n"
	 "Extensions:\n",
	 HGE_glGetString(GL_VENDOR),
	 HGE_glGetString(GL_RENDERER),
	 HGE_glGetString(GL_VERSION),
	 config->red_size,
	 config->green_size,
	 config->blue_size,
	 config->alpha_size,
	 config->doublebuffer);

  extlist = strdup((char*)HGE_glGetString(GL_EXTENSIONS));

  ext = strtok(extlist, " ");
  while(ext)
    {
      printf(" %s\n", ext);
      ext = strtok(NULL, " ");
    }

  free(extlist);

  wglGetExtensionsStringEXT = SDL_GL_GetProcAddress("wglGetExtensionsStringEXT");
  if(wglGetExtensionsStringEXT != NULL)
    {
      extlist = strdup(wglGetExtensionsStringEXT());
      ext = strtok(extlist, " ");
      while(ext)
	{
	  printf(" %s\n", ext);
	  ext = strtok(NULL, " ");
	}
      
      free(extlist);
    }
}

void
HGE_gl_find_extensions(void)
{
  if(HGE_gl_extension_supported("GL_ARB_texture_non_power_of_two"))
    global_config.gl_flags |= NON_POWER_OF_2_EXT;

  if(HGE_gl_extension_supported("GL_ARB_texture_compression") ||
     HGE_gl_extension_supported("GL_3DFX_texture_compression_FXT1") ||
     HGE_gl_extension_supported("GL_ATI_texture_compression_3dc") ||
     HGE_gl_extension_supported("GL_EXT_texture_compression_dxt1") ||
     HGE_gl_extension_supported("GL_EXT_texture_compression_s3tc") ||
     HGE_gl_extension_supported("GL_NV_texture_compression_vtc") ||
     HGE_gl_extension_supported("GL_S3_s3tc"))
    global_config.gl_flags |= TEXTURE_COMPRESSION_EXT;
}

/* code taken from opengl.org */
int
HGE_gl_extension_supported(const char* extension)
{
  const GLubyte *extensions = NULL;
  const GLubyte *start;
  GLubyte *where, *terminator;

  /* Extension names should not have spaces. */
  where = (GLubyte*)strchr(extension, ' ');
  if(where || *extension == '\0')
    return 0;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  extensions = HGE_glGetString(GL_EXTENSIONS);
  start = extensions;
  while(HGE_TRUE)
    {
      where = (GLubyte*) strstr((const char*)start, extension);
      if (!where)
	break;

      terminator = where + strlen(extension);
      if(where == start || *(where - 1) == ' ')
	if(*terminator == ' ' || *terminator == '\0')
	  return 1;
      start = terminator;
    }
  
  return 0;
}

/* Load GL functions */
void (APIENTRY* HGE_glClear)(GLbitfield);
void (APIENTRY* HGE_glBegin)(GLenum);
void (APIENTRY* HGE_glEnd)(void);
void (APIENTRY* HGE_glEnable)(GLenum);
void (APIENTRY* HGE_glDisable)(GLenum);
void (APIENTRY* HGE_glLoadIdentity)(void);
void (APIENTRY* HGE_glOrtho)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
void (APIENTRY* HGE_glPushAttrib)(GLbitfield);
void (APIENTRY* HGE_glPopAttrib)(void);
void (APIENTRY* HGE_glPushMatrix)(void);
void (APIENTRY* HGE_glPopMatrix)(void);
void (APIENTRY* HGE_glMatrixMode)(GLenum);
void (APIENTRY* HGE_glBindTexture)(GLenum,GLuint);
void (APIENTRY* HGE_glViewport)(GLint,GLint,GLsizei,GLsizei);
void (APIENTRY* HGE_glTranslatef)(GLfloat,GLfloat,GLfloat);
void (APIENTRY* HGE_glScissor)(GLint,GLint,GLsizei,GLsizei);
void (APIENTRY* HGE_glGetIntegerv)(GLenum,GLint*);
void (APIENTRY* HGE_glShadeModel)(GLenum);
void (APIENTRY* HGE_glPixelStorei)(GLenum,GLint);
void (APIENTRY* HGE_glHint)(GLenum,GLenum);
void (APIENTRY* HGE_glGetTexImage)(GLenum,GLint,GLenum,GLenum,GLvoid*);
void (APIENTRY* HGE_glGetTexParameteriv)(GLenum,GLenum,GLint*);
void (APIENTRY* HGE_glGetTexLevelParameteriv)(GLenum,GLint,GLenum,GLint*);
void (APIENTRY* HGE_glGetTexLevelParameterfv)(GLenum,GLint,GLenum,GLfloat*);
void (APIENTRY* HGE_glTexImage2D)(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*);
void (APIENTRY* HGE_glTexSubImage2D)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*);
void (APIENTRY* HGE_glGenTextures)(GLsizei,GLuint*);
void (APIENTRY* HGE_glTexParameteri)(GLenum,GLenum,GLint);
GLenum (APIENTRY* HGE_glGetError)(void);
void (APIENTRY* HGE_glVertex2i)(GLint,GLint);
void (APIENTRY* HGE_glVertex2f)(GLfloat,GLfloat);
void (APIENTRY* HGE_glVertex2iv)(GLint*);
void (APIENTRY* HGE_glVertex2fv)(GLfloat*);
void (APIENTRY* HGE_glTexCoord2i)(GLint,GLint);
void (APIENTRY* HGE_glTexCoord2f)(GLfloat,GLfloat);
void (APIENTRY* HGE_glEnableClientState)(GLenum);
void (APIENTRY* HGE_glDisableClientState)(GLenum);
void (APIENTRY* HGE_glPushClientAttrib)(GLbitfield);
void (APIENTRY* HGE_glPopClientAttrib)(void);
void (APIENTRY* HGE_glVertexPointer)(GLint,GLenum,GLsizei,const GLvoid*);
void (APIENTRY* HGE_glColorPointer)(GLint,GLenum,GLsizei,const GLvoid*);
void (APIENTRY* HGE_glDrawArrays)(GLenum,GLint,GLsizei);
void (APIENTRY* HGE_glPointSize)(GLint);
void (APIENTRY* HGE_glLineWidth)(GLint);
void (APIENTRY* HGE_glTexEnvf)(GLenum,GLenum,GLfloat);
void (APIENTRY* HGE_glColor3ub)(GLubyte,GLubyte,GLubyte);
void (APIENTRY* HGE_glColor3ubv)(const GLubyte*);
void (APIENTRY* HGE_glColor4ub)(GLubyte,GLubyte,GLubyte,GLubyte);
void (APIENTRY* HGE_glColor4ubv)(const GLubyte*);
void (APIENTRY* HGE_glBlendFunc)(GLenum,GLenum);
void (APIENTRY* HGE_glReadPixels)(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,GLvoid*);
void (APIENTRY* HGE_glRotatef)(GLfloat,GLfloat,GLfloat,GLfloat);
void (APIENTRY* HGE_glScalef)(GLfloat,GLfloat,GLfloat);
const GLubyte* (APIENTRY* HGE_glGetString)(GLenum);
void (APIENTRY* HGE_glFrustum)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
void (APIENTRY* HGE_glRecti)(GLint,GLint,GLint,GLint);
void (APIENTRY* HGE_glRectf)(GLfloat,GLfloat,GLfloat,GLfloat);
void (APIENTRY* HGE_glGetBooleanv)(GLenum,GLboolean*);
void (APIENTRY* HGE_glGetIntegerv)(GLenum,GLint*);
void (APIENTRY* HGE_glGetFloatv)(GLenum,GLfloat*);
void (APIENTRY* HGE_glGetDoublev)(GLenum,GLdouble*);

#define HGE_LOAD_GL_FUNC(FUNC) \
      { HGE_##FUNC = SDL_GL_GetProcAddress(#FUNC); if(HGE_##FUNC == NULL) goto HGE_LOAD_GL_FUNC_ERROR; }

HGE_error_t
HGE_gl_load_opengl(void)
{
  HGE_LOAD_GL_FUNC(glClear);
  HGE_LOAD_GL_FUNC(glBegin);
  HGE_LOAD_GL_FUNC(glEnd);
  HGE_LOAD_GL_FUNC(glEnable);
  HGE_LOAD_GL_FUNC(glDisable);
  HGE_LOAD_GL_FUNC(glLoadIdentity);
  HGE_LOAD_GL_FUNC(glScissor);
  HGE_LOAD_GL_FUNC(glOrtho);
  HGE_LOAD_GL_FUNC(glPushAttrib);
  HGE_LOAD_GL_FUNC(glPopAttrib);
  HGE_LOAD_GL_FUNC(glPushMatrix);
  HGE_LOAD_GL_FUNC(glPopMatrix);
  HGE_LOAD_GL_FUNC(glMatrixMode);
  HGE_LOAD_GL_FUNC(glBindTexture);
  HGE_LOAD_GL_FUNC(glViewport);
  HGE_LOAD_GL_FUNC(glTranslatef);
  HGE_LOAD_GL_FUNC(glGetIntegerv);
  HGE_LOAD_GL_FUNC(glShadeModel);
  HGE_LOAD_GL_FUNC(glPixelStorei);
  HGE_LOAD_GL_FUNC(glHint);
  HGE_LOAD_GL_FUNC(glGetTexImage);
  HGE_LOAD_GL_FUNC(glGetTexParameteriv);
  HGE_LOAD_GL_FUNC(glTexImage2D);
  HGE_LOAD_GL_FUNC(glTexSubImage2D);
  HGE_LOAD_GL_FUNC(glGenTextures);
  HGE_LOAD_GL_FUNC(glTexParameteri);
  HGE_LOAD_GL_FUNC(glGetError);
  HGE_LOAD_GL_FUNC(glGetTexLevelParameteriv);
  HGE_LOAD_GL_FUNC(glGetTexLevelParameterfv);
  HGE_LOAD_GL_FUNC(glVertex2i);
  HGE_LOAD_GL_FUNC(glVertex2f);
  HGE_LOAD_GL_FUNC(glVertex2iv);
  HGE_LOAD_GL_FUNC(glVertex2fv);
  HGE_LOAD_GL_FUNC(glTexCoord2i);
  HGE_LOAD_GL_FUNC(glTexCoord2f);
  HGE_LOAD_GL_FUNC(glEnableClientState);
  HGE_LOAD_GL_FUNC(glDisableClientState);
  HGE_LOAD_GL_FUNC(glPushClientAttrib);
  HGE_LOAD_GL_FUNC(glPopClientAttrib);
  HGE_LOAD_GL_FUNC(glVertexPointer);
  HGE_LOAD_GL_FUNC(glColorPointer);
  HGE_LOAD_GL_FUNC(glDrawArrays);
  HGE_LOAD_GL_FUNC(glPointSize);
  HGE_LOAD_GL_FUNC(glLineWidth);
  HGE_LOAD_GL_FUNC(glTexEnvf);
  HGE_LOAD_GL_FUNC(glColor3ub);
  HGE_LOAD_GL_FUNC(glColor3ubv);
  HGE_LOAD_GL_FUNC(glColor4ub);
  HGE_LOAD_GL_FUNC(glColor4ubv);
  HGE_LOAD_GL_FUNC(glBlendFunc);
  HGE_LOAD_GL_FUNC(glReadPixels);
  HGE_LOAD_GL_FUNC(glRotatef);
  HGE_LOAD_GL_FUNC(glScalef);
  HGE_LOAD_GL_FUNC(glGetString);
  HGE_LOAD_GL_FUNC(glFrustum);
  HGE_LOAD_GL_FUNC(glRecti);
  HGE_LOAD_GL_FUNC(glRectf);
  HGE_LOAD_GL_FUNC(glGetBooleanv);
  HGE_LOAD_GL_FUNC(glGetIntegerv);
  HGE_LOAD_GL_FUNC(glGetFloatv);
  HGE_LOAD_GL_FUNC(glGetDoublev);
  
  return HGE_SUCCESS;

 HGE_LOAD_GL_FUNC_ERROR:
  return HGE_FAILURE;
}
