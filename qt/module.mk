LIBS += $(shell pkg-config QtGui --libs)

QTMOC := qt/application.cpp \
	 qt/runqueuemonitor.cpp \
	 qt/pythonterminalview.cpp \
	 qt/runtimemodel.cpp

QTUI =  qt/application.ui

QTSRC := qt/application.cpp \
	 qt/guiproxy.cpp \
	 qt/guiglue.cpp \
	 qt/dialog.cpp \
	 qt/codeview.cpp \
	 qt/codemodel.cpp \
	 qt/dataview.cpp \
	 qt/symbolview.cpp \
	 qt/symbolmodel.cpp \
	 qt/main.cpp \
	 qt/multilinedialog.cpp \
	 qt/datatypeview.cpp \
	 qt/main.cpp \
	 $(QTMOC) \
	 $(QTMOC:.cpp=.moc.cpp)

SRC += $(QTSRC)

QTOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(QTSRC))
QTDEPS := $(patsubst %.cpp,$(BUILDDIR)/%.d, $(QTSRC))
QTTARG := $(QTOBJS) $(QTDEPS)

ifeq ($(OS),Darwin)
$(QTTARG): CPPFLAGS += -Wno-deprecated -I/Library/Frameworks/QtGui.framework/Headers/ -I/Library/Frameworks/QtCore.framework/Headers/
LDFLAGS += -framework QtGui -framework QtCore
else
$(QTTARG): CPPFLAGS += $(shell pkg-config QtGui --cflags) -Wno-deprecated
endif

%.ui.h: %.ui
	@echo "UIC	$<"
	@uic -o $@ $<

%.moc.cpp: %.h
	@echo "MOC	$@"
	@moc -o $@ $<
