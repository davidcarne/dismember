MBSRC := memorymodel/memorybackedprojectmodel.cpp memorymodel/memlocdata.cpp

SRC += $(MBSRC)

MBOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(MBSRC))
MBDEPS := $(patsubst %.cpp,$(BUILDDIR)/%.d, $(MBSRC))
MBTARG := $(MBOBJS) $(MBDEPS)
