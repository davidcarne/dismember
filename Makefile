OS = $(shell uname)

ifeq ($(OS),Darwin)
	include Makefile.macosx
else
	include Makefile.generic
endif
