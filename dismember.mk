


MODULES := python_embed arch loaders $(GUI)

INCDIRS := $(MODULES)
INCPATHS := -I.

LIBS :=
SRC := 	abstractdata.cpp comment.cpp memlocdata.cpp xref.cpp \
	datatypereg.cpp memlocmanager.cpp run_queue.cpp xref_build.cpp \
	app_main.cpp document.cpp memsegment.cpp stringconstant.cpp \
	xrefmanager.cpp binaryconstant.cpp dsmem_trace.cpp \
	memsegmentmanager.cpp symlist.cpp callback.cpp \
	codeblock.cpp instruction.cpp program_flow_analysis.cpp


BUILDDIR := build
#DEPSFILE := $(BUILDDIR)/deps.mk
PROG := $(BUILDDIR)/dismember

# Default build target
all: $(PROG)

include $(patsubst %, %/module.mk, $(MODULES))


INCPATHS += $(patsubst %, -I%, $(INCDIRS))

CPPSRCS := $(filter %.cpp, $(SRC) )
CPPOBJS := $(patsubst %.cpp,%.o, $(CPPSRCS) )

CPPDEFS = -DDISABLE_ADDRESS_T_HASH
CPPFLAGS += -Wall -Wno-unused  -Wno-unknown-pragmas -Wno-reorder \
	    -Wno-non-virtual-dtor -g $(CPPDEFS)


PYEXTCPP := $(shell python-config --cflags)
PYEXTLD := $(shell python-config --libs)
EXTCPP += $(PYEXTCPP)

CPPFLAGS += $(INCPATHS) $(EXTCPP)
LIBS += -lboost_python-mt -lboost_thread-mt -lboost_serialization-mt `python-config --cflags` -lpthread $(PYEXTLD)

$(PROG): $(CPPOBJS)
	mkdir -p $(BUILDDIR)
	g++ $(LDFLAGS) $(LIBS) $^ -o $@

#$(DEPSFILE): $(CPPSRC)
#	gccmakedep -f $(DEPSFILE) $(CPPSRCS)

include $(DEPSFILE)


clean:
	rm -f $(DEPSFILE) $(CPPOBJS)
