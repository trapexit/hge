SDL_CFLAGS=$(shell sdl-config --cflags)
SDL_LIBS=$(shell sdl-config --libs)
OGL_LIB = GL
CFLAGS = -g -Wall -Werror -I../include -O0 $(SDL_CFLAGS)
LIBFLAGS = -L../lib -L. -L/usr/X11R6/lib 
LIBS = -lHGE -lSDL_mixer -lSDL_image $(SDL_LIBS) -lpng -lz # -l${OGL_LIB}
SRCS = $(shell echo *.c)
OBJS = ${SRCS:.c=.o}
BINS = ${SRCS:.c=}
EXT = 

ifeq ($(CC),i386-mingw32msvc-gcc)
	OGL_LIB = opengl32
	EXT = ".exe"
endif

all: $(BINS)

.c:
	$(CC) $< -o $@${EXT} ${CFLAGS} ${LIBFLAGS} ${LIBS}	

clean:
	rm -f ${OBJS} ${BINS} *.exe *~ Makefile.bak

depend:
	makedepend -Y -- ${CFLAGS} -- ${SRCS}
