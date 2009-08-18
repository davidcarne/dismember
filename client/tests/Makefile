
runtests: kvs_tests
	./kvs_tests


kvs_tests: kvs_tests.cpp ../kvs/*.cpp ../kvs/*.h
	g++ -I../ -I../kvs kvs_tests.cpp ../kvs/*.cpp -o kvs_tests -lboost_test_exec_monitor