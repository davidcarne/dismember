LIBS += $(shell pkg-config QtGui --libs)

QTMOC := qt/application.cpp \
	 qt/runqueuemonitor.cpp \
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
	 qt/pythonterminalview.cpp \
	 qt/main.cpp \
	 $(QTMOC) \
	 $(QTMOC:.cpp=.moc.cpp)

SRC += $(QTSRC)

ifeq ($(OS),Darwin)
$(QTSRC:.cpp=.o): CPPFLAGS += -Wno-deprecated -I/Library/Frameworks/QtGui.framework/Headers/ -I/Library/Frameworks/QtCore.framework/Headers/
LDFLAGS += -framework QtGui -framework QtCore
else
$(QTSRC:.cpp=.o): CPPFLAGS += $(shell pkg-config QtGui --cflags) -Wno-deprecated
endif

$(QTSRC:.cpp=.o): $(QTUI:.ui=.ui.h)

%.ui.h: %.ui
	uic -o $@ $<

%.moc.cpp: %.h
	moc -o $@ $<
