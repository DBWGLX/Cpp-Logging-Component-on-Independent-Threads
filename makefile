all: test.out

test.out: Level.cpp LogStarter.cc LogInfo.cc test.cc 
	g++ $^ -o $@  -std=c++17 -pthread -g 

.PHONY: clean

clean:
	rm -f test.out