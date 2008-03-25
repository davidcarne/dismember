OS = $(shell uname)

ifeq ($(OS),Darwin)
	include Makefile.macosx
else
	include Makfile.generic
endif