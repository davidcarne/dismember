


MODULES := python_embed arch loaders $(GUI)

INCDIRS := $(MODULES)
INCPATHS := -I.

LIBS :=
SRC := 	comment.cpp memlocdata.cpp xref.cpp symbol_analysis.cpp \
	datatypereg.cpp memlocmanager.cpp run_queue.cpp exception.cpp \
	app_main.cpp document.cpp memsegment.cpp stringconstant.cpp \
	xrefmanager.cpp binaryconstant.cpp dsmem_trace.cpp \
	memsegmentmanager.cpp symlist.cpp callback.cpp address.cpp \
	codeblock.cpp instruction.cpp program_flow_analysis.cpp


BUILDDIR := build
PROG := $(BUILDDIR)/dismember

# Default build target
all: $(PROG)

include $(patsubst %, %/module.mk, $(MODULES))


INCPATHS += $(patsubst %, -I%, $(INCDIRS))

CPPSRCS := $(filter %.cpp, $(SRC) )
CPPOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(CPPSRCS) )
CPPDEPS := $(CPPOBJS:.o=.d)

CPPDEFS = -DDISABLE_ADDRESS_T_HASH
CPPFLAGS += -Wall -Wno-unused  -Wno-unknown-pragmas -Wno-reorder \
	    -Wno-non-virtual-dtor -g $(CPPDEFS)


# Some python implementations add flags they shouldn't in --cflags
PYEXTCPP := $(shell python-config --cflags | sed s/-Wstrict-prototypes//g)
PYEXTLD := $(shell python-config --libs)
EXTCPP += $(PYEXTCPP)

CPPFLAGS += $(INCPATHS) $(EXTCPP)
LIBS += -lboost_python-mt -lboost_thread-mt -lboost_serialization-mt -lpthread $(PYEXTLD)

$(PROG): $(CPPOBJS)
	@echo "LD	$@"
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@

$(BUILDDIR)/%.o: %.cpp
	@echo "CXX	$<"
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

$(BUILDDIR)/%.d: %.cpp
	@echo "DEP	$<"
	@mkdir -p $(@D)
	@$(CXX) -MM $(CPPFLAGS) $< | sed -e "s@^\(.*\)\.o:@$(@D)/\1.d $(@D)/\1.o:@" > $@

-include $(CPPDEPS)


clean:
	@echo CLEAN
	@$(RM) $(CPPOBJS) $(PROG)

distclean:
	@echo DISTCLEAN
	@$(RM) $(CPPDEPS) $(CPPOBJS) $(PROG)
