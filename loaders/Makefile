LOADERS := $(shell for i in */; do if test -f $$i/Makefile; then echo $$i | sed 's|/$$||'; fi; done)
OBJ = loaderfactory.o
CPPFLAGS +=  -I../ -g

all: loaders.o

loaders.o: $(OBJ) .always
	@set -e; for i in $(LOADERS); do \
		make -C $$i; \
	done
	find . -name built-in.o | xargs $(LD) -r $(OBJ) -o $@

clean:
	@set -e; for i in $(LOADERS); do \
		make -C $$i clean; \
	done
	$(RM) $(OBJ)

.always: ;
