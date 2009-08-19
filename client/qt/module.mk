

QTMOC := qt/application.cpp \
	 qt/runqueuemonitor.cpp \
	 qt/pythonterminalview.cpp \
	 qt/searchwidget.cpp \
	 qt/runtimemodel.cpp

QTUI =  qt/application.ui
QTRC =  qt/resources.qrc

QTSRC := qt/application.cpp \
	 qt/guiproxy.cpp \
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
	 $(QTRC:.qrc=.rcc.cpp) \
	 $(QTMOC:.cpp=.moc.cpp)

SRC += $(QTSRC)

QTOBJS := $(patsubst %.cpp,$(BUILDDIR)/%.o, $(QTSRC))
QTDEPS := $(patsubst %.cpp,$(BUILDDIR)/%.d, $(QTSRC))
QTTARG := $(QTOBJS) $(QTDEPS)

ifeq ($(OS),Darwin)
$(QTTARG): CPPFLAGS += -Wno-deprecated -I/Library/Frameworks/QtGui.framework/Headers/ -I/Library/Frameworks/QtCore.framework/Headers/
LDFLAGS += -framework QtGui -framework QtCore
else
LIBS += $(shell pkg-config QtGui --libs)
$(QTTARG): CPPFLAGS += $(shell pkg-config QtGui --cflags) -Wno-deprecated
endif

$(QTUI:.ui=.h): $(QTUI:.ui=.ui.h)

%.ui.h: %.ui
	@echo "UIC	$<"
	@uic -o $@ $<

%.moc.cpp: %.h
	@echo "MOC	$@"
	@moc -o $@ $<

%.rcc.cpp: %.qrc
	@echo "RCC	$@"
	@rcc -o $@ $<
