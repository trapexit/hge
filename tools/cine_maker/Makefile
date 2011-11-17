CFLAGS = -g -Wall `sdl-config --cflags` -I../..
LIBS   = -L../.. -lHGE -lSDL_mixer -lSDL_image `sdl-config --libs`

PROGRAM=cine_maker
OBJFILES=cine_maker.o

$(PROGRAM): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) $(LIBS) -o $(PROGRAM) 

cine_makerr.o: ../../HGE.h ../../libHGE.a

clean:
	rm -f *~ *.o $(PROGRAM)