CFLAGS = -g -Wall `sdl-config --cflags` -I../..
LIBS   = -L../.. -lHGE -lSDL_mixer -lSDL_image `sdl-config --libs`

PROGRAM=packer
OBJFILES=packer.o

$(PROGRAM): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) $(LIBS) -o $(PROGRAM) 

packer.o: ../../HGE.h ../../libHGE.a

clean:
	rm -f *~ *.o $(PROGRAM)