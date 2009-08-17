runtests: tests
	./tests

tests: tests.cpp ../../kvs/*
	g++ -g -Wno-deprecated -I. -I../../kvs/ -I../../ tests.cpp ../../kvs/*.cpp -o tests

tests.cpp:	basictests.h
	./cxxtestgen.py --have-eh --abort-on-fail --error-printer -o tests.cpp basictests.h 
