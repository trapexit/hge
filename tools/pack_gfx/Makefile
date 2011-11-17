CFLAGS = -g -Wall -O2 `sdl-config --cflags` -I../..

PROGRAM=pack_gfx
OBJFILES=pack_gfx.o

$(PROGRAM): $(OBJFILES)
	$(CC) $(CFLAGS) $(LIBS) -o $(PROGRAM) $(OBJFILES)

pack_gfx.o: ../../HGE_types.h

clean:
	rm -f *~ *.o $(PROGRAM)