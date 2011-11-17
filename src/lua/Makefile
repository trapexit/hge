VER = $(shell svn info | grep Revision | cut -f 2 -d ' ')
DEGREES = -DUSE_DEGREES
NUMBER = -DLUA_USER_H='"luser_number.h"' -DUSE_FLOAT
SDL_CFLAGS = $(shell sdl-config --cflags)
OPTS = -O0

CFLAGS = -g -Wall ${OPTS} -I. -I../../include $(DEGREES) $(NUMBER) $(SDL_CFLAGS)
SRCS = $(shell echo *.c)
OBJS = ${SRCS:.c=.o}

all: libHGElua.a headers

.c.o:
	${CC} ${CFLAGS} -o $@ -c $<

libHGElua.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	$(AR) -s $@

headers:
	cp lua.h lualib.h lauxlib.h lua_hge.h lua_sdl.h lua_gl.h ../../include
	sh create_hgelua_h.sh

clean:
	rm -f ${OBJS} *~ *.{a,so*} Makefile.bak

depend:
	makedepend -Y -- ${CFLAGS} -- ${SRCS}
