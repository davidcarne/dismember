OBJS=arm/arm.o

all: all-recursive archs.o

archs.o: $(OBJS)
	ld -r $^ -o $@

# this makefile is a hack. fixme

all-recursive: 
	make -C arm

clean:
	make -C arm clean
	rm -f archs.o