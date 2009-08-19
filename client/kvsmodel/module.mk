MBSRC := kvsmodel/kvs_comment.cpp kvsmodel/kvs_memlocdata.cpp kvsmodel/kvs_projectmodel.cpp kvsmodel/kvs_serializers.cpp

SRC += $(MBSRC)

MBOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(MBSRC))
MBDEPS := $(patsubst %.cpp,$(BUILDDIR)/%.d, $(MBSRC))
MBTARG := $(MBOBJS) $(MBDEPS)
