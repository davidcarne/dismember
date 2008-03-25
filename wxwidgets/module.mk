
WXUNI := $(shell echo `wx-config --unicode=no 2>&1` | grep -o unicode)
ifeq ($(WXUNI),unicode)
	# double assignment speeds up build
	WXCPPFLAGS := $(shell wx-config --cflags)
	CPPFLAGS += $(WXCPPFLAGS) 
	WXLIBS := $(shell wx-config --libs)
	LIBS += $(WXLIBS)
else
	WXCPPFLAGS := $(shell wx-config --cflags --unicode=no)
	CPPFLAGS += $(WXCPPFLAGS) 
	WXLIBS := $(shell wx-config --libs --unicode=no)
	LIBS += $(WXLIBS)
endif


SRC += 	wxwidgets/codeviewcanvas.cpp wxwidgets/dataview.cpp wxwidgets/documentwindow.cpp \
	wxwidgets/guiproxy.cpp wxwidgets/pythonterminalview.cpp wxwidgets/toolbar.cpp \
	wxwidgets/datatypelist.cpp wxwidgets/dialog.cpp wxwidgets/guiglue.cpp \
	wxwidgets/main.cpp wxwidgets/routinelist.cpp


WXVER=$(shell wx-config --release)

# Enable an internal WXAUI if the existing one is missing
ifneq ($(WXVER),2.8)
SRC += wxaui.cpp
CPPFLAGS += -DWXAUI=1
endif

#ifeq ($(ARCH),Darwin)
	#/Developer/Tools/Rez -d __DARWIN__ -t APPL -d __WXMAC__  -d WXUSINGDLL -o $@ Carbon.r
#endif


