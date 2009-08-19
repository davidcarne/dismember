
BINDINGSSRC :=  bind_analysis.cpp bind_codeblocks.cpp bind_document.cpp \
		bind_trace.cpp bindings.cpp \
		bind_arch.cpp bind_datatype.cpp	\
		bind_runqueue.cpp bind_types.cpp

PYEMBEDSRC := globalpythoninterpreter.cpp localpythoninterpreter.cpp $(addprefix bindings/, $(BINDINGSSRC))

SRC += $(addprefix python_embed/, $(PYEMBEDSRC))
TEST_SRC += $(addprefix python_embed/, $(PYEMBEDSRC))
INCPATHS += -Ipython_embed/bindings