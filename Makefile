ARCH = $(shell uname)
PROG = dismember


CC ?= g++
CPPFLAGS += -g -Icontrib/ `xml2-config --cflags` -Iloaders/

ARMPREFIX ?= arm-uclinux-elf
ARMAS ?= $(ARMPREFIX)-as
ARMLD ?= $(ARMPREFIX)-ld
ARMOC ?= $(ARMPREFIX)-objcopy

OBJS = trace.o ramops.o code_follow.o xref_build.o  \
	symlist.o traceloadsave.o xref.o contrib/base64.o \
	memlocdata.o binaryconstant.o xmlbzip2_fileio.o memsegment.o \
	callback.o abstractdata.o comment.o datatypereg.o stringconstant.o

GUI=wxwidgets

ifeq ($(ARCH),Darwin)
all: $(PROG).app
else
all: $(PROG)
endif


$(PROG): $(OBJS) .always
	make -C arch
	make -C loaders
	make -C $(GUI) PROG="$@" OOBJS="$(addprefix ../, $(OBJS)) ../arch/archs.o ../loaders/loaders.o"

$(OBJS): %.o: %.cpp trace.h symlist.h memlocdata.h #memtags.h

app.bin: app.s
	$(ARMAS) -o app.o app.s
	$(ARMLD) -e _start -o app app.o
	$(ARMOC) -O binary app app.bin

clean:
	$(RM) $(PROG) app app.bin app.o $(OBJS)
	$(RM) -r $(PROG).app
	make -C arch clean
	make -C loaders clean
	make -C $(GUI) clean PROG="$(PROG)"

$(PROG).app: $(PROG)
	mkdir -p $(PROG).app
	mkdir -p $(PROG).app/Contents
	mkdir -p $(PROG).app/Contents/MacOS
	echo -n 'APPL????' > $(PROG).app/Contents/PkgInfo
	cp $(PROG) $(PROG).app/Contents/MacOS

.always: ;

doc:
	doxygen
