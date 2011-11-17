#ifndef _HGE_GL_FUNCS_H_
#define _HGE_GL_FUNCS_H_

#include "SDL_opengl.h"

extern void (APIENTRY* HGE_glClear)(GLbitfield);
extern void (APIENTRY* HGE_glBegin)(GLenum);
extern void (APIENTRY* HGE_glEnd)(void);
extern void (APIENTRY* HGE_glEnable)(GLenum);
extern void (APIENTRY* HGE_glDisable)(GLenum);
extern void (APIENTRY* HGE_glLoadIdentity)(void);
extern void (APIENTRY* HGE_glOrtho)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
extern void (APIENTRY* HGE_glPushAttrib)(GLbitfield);
extern void (APIENTRY* HGE_glPopAttrib)(void);
extern void (APIENTRY* HGE_glPushMatrix)(void);
extern void (APIENTRY* HGE_glPopMatrix)(void);
extern void (APIENTRY* HGE_glMatrixMode)(GLenum);
extern void (APIENTRY* HGE_glBindTexture)(GLenum,GLuint);
extern void (APIENTRY* HGE_glViewport)(GLint,GLint,GLsizei,GLsizei);
extern void (APIENTRY* HGE_glTranslatef)(GLfloat,GLfloat,GLfloat);
extern void (APIENTRY* HGE_glScissor)(GLint,GLint,GLsizei,GLsizei);
extern void (APIENTRY* HGE_glGetIntegerv)(GLenum,GLint*);
extern void (APIENTRY* HGE_glShadeModel)(GLenum);
extern void (APIENTRY* HGE_glPixelStorei)(GLenum,GLint);
extern void (APIENTRY* HGE_glHint)(GLenum,GLenum);
extern void (APIENTRY* HGE_glGetTexImage)(GLenum,GLint,GLenum,GLenum,GLvoid*);
extern void (APIENTRY* HGE_glGetTexParameteriv)(GLenum,GLenum,GLint*);
extern void (APIENTRY* HGE_glGetTexLevelParameteriv)(GLenum,GLint,GLenum,GLint*);
extern void (APIENTRY* HGE_glGetTexLevelParameterfv)(GLenum,GLint,GLenum,GLfloat*);
extern void (APIENTRY* HGE_glTexImage2D)(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*);
extern void (APIENTRY* HGE_glTexSubImage2D)(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*);
extern void (APIENTRY* HGE_glGenTextures)(GLsizei,GLuint*);
extern void (APIENTRY* HGE_glTexParameteri)(GLenum,GLenum,GLint);
extern GLenum (APIENTRY* HGE_glGetError)(void);
extern void (APIENTRY* HGE_glVertex2i)(GLint,GLint);
extern void (APIENTRY* HGE_glVertex2f)(GLfloat,GLfloat);
extern void (APIENTRY* HGE_glVertex2iv)(GLint*);
extern void (APIENTRY* HGE_glVertex2fv)(GLfloat*);
extern void (APIENTRY* HGE_glTexCoord2i)(GLint,GLint);
extern void (APIENTRY* HGE_glTexCoord2f)(GLfloat,GLfloat);
extern void (APIENTRY* HGE_glEnableClientState)(GLenum);
extern void (APIENTRY* HGE_glDisableClientState)(GLenum);
extern void (APIENTRY* HGE_glPushClientAttrib)(GLbitfield);
extern void (APIENTRY* HGE_glPopClientAttrib)(void);
extern void (APIENTRY* HGE_glVertexPointer)(GLint,GLenum,GLsizei,const GLvoid*);
extern void (APIENTRY* HGE_glColorPointer)(GLint,GLenum,GLsizei,const GLvoid*);
extern void (APIENTRY* HGE_glDrawArrays)(GLenum,GLint,GLsizei);
extern void (APIENTRY* HGE_glPointSize)(GLint);
extern void (APIENTRY* HGE_glLineWidth)(GLint);
extern void (APIENTRY* HGE_glTexEnvf)(GLenum,GLenum,GLfloat);
extern void (APIENTRY* HGE_glColor3ub)(GLubyte,GLubyte,GLubyte);
extern void (APIENTRY* HGE_glColor3ubv)(const GLubyte*);
extern void (APIENTRY* HGE_glColor4ub)(GLubyte,GLubyte,GLubyte,GLubyte);
extern void (APIENTRY* HGE_glColor4ubv)(const GLubyte*);
extern void (APIENTRY* HGE_glBlendFunc)(GLenum,GLenum);
extern void (APIENTRY* HGE_glReadPixels)(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,GLvoid*);
extern void (APIENTRY* HGE_glRotatef)(GLfloat,GLfloat,GLfloat,GLfloat);
extern void (APIENTRY* HGE_glScalef)(GLfloat,GLfloat,GLfloat);
extern const GLubyte* (APIENTRY* HGE_glGetString)(GLenum);
extern void (APIENTRY* HGE_glFrustum)(GLdouble,GLdouble,GLdouble,GLdouble,GLdouble,GLdouble);
extern void (APIENTRY* HGE_glRecti)(GLint,GLint,GLint,GLint);
extern void (APIENTRY* HGE_glRectf)(GLfloat,GLfloat,GLfloat,GLfloat);
extern void (APIENTRY* HGE_glGetBooleanv)(GLenum,GLboolean*);
extern void (APIENTRY* HGE_glGetIntegerv)(GLenum,GLint*);
extern void (APIENTRY* HGE_glGetFloatv)(GLenum,GLfloat*);
extern void (APIENTRY* HGE_glGetDoublev)(GLenum,GLdouble*);

#endif /* _HGE_GL_FUNCS_H_ */
