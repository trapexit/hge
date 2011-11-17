VER=$(shell svn info | grep Revision | cut -f 2 -d ' ')

all: libHGE libHGElua

libHGE:
	cd src && $(MAKE)
	cp src/libHGE.a lib
	cp src/*.h include        

libHGElua:
	cd src/lua && $(MAKE)
	cp src/lua/libHGElua.a lib
	cp src/lua/HGElua.h include

clean:
	cd src && $(MAKE) clean
	cd src/lua && $(MAKE) clean
	rm -rf lib/*
	rm -rf include/*

dist-clean: clean
	cd test && $(MAKE) clean
	cd demos && $(MAKE) clean

release: clean
	mkdir -p ../libhge-${VER}
	cp -r . ../libhge-${VER}
	find ../libhge-${VER} -type d -name .svn -print | xargs -n 1 rm -rf
	tar cfj ../libhge-${VER}.tar.bz2 ../libhge-${VER}
	rm -rf ../libhge-${VER}
