OPT = -O0
SDL_CFLAGS = $(shell sdl-config --cflags)
CFLAGS = -g -Wall -Werror $(SDL_CFLAGS) $(OPT)
SRCS = $(shell echo HGE*.c)
OBJS = ${SRCS:.c=.o}
VER = $(shell svn info | grep Revision | cut -f 2 -d ' ')


all: libHGE.a headers

.c.o:
	${CC} ${CFLAGS} -o $@ -c $<

libHGE.a: ${OBJS}
	${AR} ${ARFLAGS} $@ ${OBJS}
	$(AR) -s $@

headers:
	sh create_hge_h.sh

clean:
	rm -f ${OBJS} *~ libHGE.a Makefile.bak

depend:
	makedepend -Y -- ${CFLAGS} -- ${SRCS}
