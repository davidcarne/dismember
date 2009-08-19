
runtests: kvs_tests
	./kvs_tests

LIBS=-lboost_test_exec_monitor
INCLUDES=-I../ -I../kvs kvs_tests.cpp -I../contrib


kvs_tests: kvs_tests.cpp ../kvs/*.cpp ../kvs/*.h ../contrib/*
	g++ -g $(INCLUDES) ../kvs/*.cpp -o kvs_tests $(LIBS)
