


MODULES := python_embed arch loaders kvsmodel datatype kvs model_interface $(GUI)

INCDIRS := $(MODULES)
INCPATHS := -I. -Icontrib

LIBS :=

COMMON_SRC := xref.cpp symbol_analysis.cpp \
	datatypereg.cpp memlocmanager.cpp run_queue.cpp exception.cpp \
	workspace.cpp \
	xrefmanager.cpp search.cpp \
        symlist.cpp address.cpp \
	program_flow_analysis.cpp guiglue.cpp
	
TEST_SRC := $(COMMON_SRC)
SRC := $(COMMON_SRC) app_main.cpp
	
BUILDDIR := build
PROG := $(BUILDDIR)/dismember


TEST := $(BUILDDIR)/run-tests

# Default build target
all: $(PROG)

run-tests: $(TEST)
	$(TEST)
	
include $(patsubst %, %/module.mk, $(MODULES))

# Do this separately; we don't want it in the include path
include tests/tests.mk

INCPATHS += $(patsubst %, -I%, $(INCDIRS))

CPPSRCS := $(filter %.cpp, $(SRC) )
CPPOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(CPPSRCS) )
CPPDEPS := $(CPPOBJS:.o=.d)

CPPTESTSRCS := $(filter %.cpp, $(TEST_SRC) )
CPPTESTOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(CPPTESTSRCS) )

CPPDEFS = -DDISABLE_ADDRESS_T_HASH

# Some python implementations add flags they shouldn't in --cflags
PYEXTCPP := $(shell python-config --includes)
PYEXTLD := $(shell python-config --libs)
EXTCPP += $(PYEXTCPP)

CPPFLAGS += $(INCPATHS) $(EXTCPP) -Wall -Wno-unknown-pragmas -Wno-reorder \
	    -g $(CPPDEFS)


CPPFLAGS += $(INCPATHS) $(EXTCPP)
LIBS += -lboost_python-mt -lboost_thread-mt -lboost_serialization-mt -lpthread $(PYEXTLD)
TEST_LIBS := -lboost_test_exec_monitor

# Before building anything; parse any dep files + delete ones that reference 
# nonexistant files [workaround a make bug in which it terminates building 
# when dep missing]
# Fake var to force execution
FOO := $(shell ./build_tools/fixup_deps.py)

$(PROG): $(CPPOBJS)
	@echo "LD	$@"
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) $(LIBS) $^ -o $@
	
$(TEST): $(CPPTESTOBJS)
	@echo "LD	$@"
	@mkdir -p $(@D)
	@$(CXX) $(LDFLAGS) $(LIBS) $(TEST_LIBS) $^ -o $@
	
$(BUILDDIR)/%.o: %.cpp
	@echo "CXX	$<"
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) -c -o $@ $<

$(BUILDDIR)/%.d: %.cpp
	@echo "DEP	$<"
	@mkdir -p $(@D)
	@#@$(CXX) -MM $(CPPFLAGS) $< | sed -e "s@^\(.*\)\.o:@$(@D)/\1.d $(@D)/\1.o:@" > $@
	@./build_tools/fast_dep.py $(CPPFLAGS) $< | sed -e "s@^\(.*\)\.o:@$(@D)/\1.d $(@D)/\1.o:@" > $@
-include $(CPPDEPS)

test:
	make -C tests

clean:
	@echo CLEAN
	@$(RM) $(CPPOBJS) $(PROG)

distclean:
	@echo DISTCLEAN
	@$(RM) $(CPPDEPS) $(CPPOBJS) $(PROG)
