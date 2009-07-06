LIBS += $(shell pkg-config QtGui --libs)

QTMOC := qt/application.cpp \
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
	 $(QTMOC) \
	 $(QTMOC:.cpp=.moc.cpp)

SRC += $(QTSRC)

$(QTSRC:.cpp=.o): CPPFLAGS += $(shell pkg-config QtGui --cflags) -Wno-deprecated

$(QTSRC:.cpp=.o): $(QTUI:.ui=.ui.h)

%.ui.h: %.ui
	uic -o $@ $<

%.moc.cpp: %.h
	moc -o $@ $<
