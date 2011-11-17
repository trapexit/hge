SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LIBS=$(shell sdl-config --libs)
CFLAGS = -g -Wall -Werror -I../include -O0 $(SDL_CFLAGS)
LIBFLAGS = -L../lib -L. -L/usr/X11R6/lib
LIBS = -lHGE -lSDL_mixer -lSDL_image $(SDL_LIBS) -lpng -lz
SRCS = $(shell echo test_*.c)
OBJS = ${SRCS:.c=.o}
BINS = ${SRCS:.c=${EXT}}

all: $(BINS)

test_lua:
	$(CC) test_lua.c -o test_lua$(EXT) $(CFLAGS) $(LIBFLAGS) -lHGElua $(LIBS)

.c:
	$(CC) $< -o $@ ${CFLAGS} ${LIBFLAGS} ${LIBS}	

clean:
	rm -f ${OBJS} ${BINS} *~ Makefile.bak

depend:
	makedepend -Y -- ${CFLAGS} -- ${SRCS}
