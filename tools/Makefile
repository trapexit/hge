all: packer cine_maker

packer:
	cd packer && $(MAKE)
packer_clean:
	cd packer && $(MAKE) clean

cine_maker:
	cd cine_maker && $(MAKE)
cine_maker_clean:
	cd cine_maker && $(MAKE) clean

clean: packer_clean cine_maker_clean
	rm -f *~ *.o