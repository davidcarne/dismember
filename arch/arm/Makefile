OBJS = arm_instruction.o arm_arch.o dec_op.o disasm.o

all: arm.o

arm.o: $(OBJS)
	ld -r $^ -o $@

$(OBJS): %.o: %.cpp ../../trace.h ../../symlist.h arm_instruction.h arm.h #memtags.h

clean:
	rm -f $(OBJS) arm.o

