CPPFLAGS += -I../ -I../../ -g

all: built-in.o

built-in.o: $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $^

clean:
	$(RM) $(OBJS) built-in.o
