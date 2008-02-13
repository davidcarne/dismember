ARCH = $(shell uname)
OO = ../${PROG}
CPPFLAGS ?= -Wall -Wno-unused -g -I..

LDFLAGS += `xml2-config --libs` -lbz2

WXUNI= $(shell echo `wx-config --unicode=no 2>&1` | grep -o unicode)
ifeq ($(WXUNI),unicode)
	CPPFLAGS += -g `wx-config --cflags`
	LDFLAGS += `wx-config --libs`
else
	CPPFLAGS += -g `wx-config --cflags --unicode=no`
	LDFLAGS += `wx-config --libs --unicode=no`
endif

OBJS = datatypelist.o codeviewcanvas.o codeview.o routinelist.o dataview.o toolbar.o main.o dialog.o guiproxy.o

WXVER=$(shell wx-config --release)

ifneq ($(WXVER),2.8)
OBJS += wxaui.o
CPPFLAGS += -DWXAUI=1
endif

all: $(OO)

$(OO): $(OBJS) ${OOBJS}
	$(CXX) $(LDFLAGS) -o $@ $^
ifeq ($(ARCH),Darwin)
	#/Developer/Tools/Rez -d __DARWIN__ -t APPL -d __WXMAC__  -d WXUSINGDLL -o $@ Carbon.r
endif

$(OBJS): %.o: %.cpp ../trace.h ../symlist.h codeview.h

clean:
	$(RM) $(OO) $(OBJS)

datatypelist.o: datatypelist.h
codeviewcanvas.o: codeviewcanvas.h
codeview.o: codeview.h
routinelist.o: routinelist.h
dataview.o: dataview.h
toolbar.o: toolbar.h
main.o: main.h
dialog.o: dialog.h
guiproxy.o: guiproxy.h
