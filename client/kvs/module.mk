MBSRC := kvs/i_kvs.cpp kvs/localkvsstore.cpp

SRC += $(MBSRC)

MBOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(MBSRC))
MBDEPS := $(patsubst %.cpp,$(BUILDDIR)/%.d, $(MBSRC))
MBTARG := $(MBOBJS) $(MBDEPS)
