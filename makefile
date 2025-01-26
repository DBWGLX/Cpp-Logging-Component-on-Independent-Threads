all: test.out

test.out: level.cpp logSink.cc logInfo.cc test.cc 
	g++ $^ -o $@  -std=c++11 -pthread -g 

.PHONY: clean

clean:
	rm -f test.out