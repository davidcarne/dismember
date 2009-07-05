LIBS += $(shell pkg-config QtGui --libs)

QTSRC := qt/documentwindow.cpp \
	 qt/guiproxy.cpp \
	 qt/guiglue.cpp \
	 qt/documentwindow.moc.cpp \
	 qt/dialog.cpp \
	 qt/codeview.cpp \
	 qt/documentproxymodel.cpp \
	 qt/main.cpp

QTUI =  qt/documentwindow.ui

SRC += $(QTSRC)

$(QTSRC:.cpp=.o): CPPFLAGS += $(shell pkg-config QtGui --cflags) -Wno-deprecated

$(QTSRC:.cpp=.o): $(QTUI:.ui=.ui.h)

%.ui.h: %.ui
	uic -o $@ $<

%.moc.cpp: %.h
	moc -o $@ $<
