main: main.cpp
	clang++ -std=c++11 main.cpp -o output
test: test.cpp
	clang++ -std=c++11 test.cpp -o test
