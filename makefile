SRC := $(wildcard *.cpp)
OUT := $(patsubst %.cpp, %.out, $(SRC))


all: $(OUT) 

%.out: %.cpp
	g++ -std=c++17 -o $@ $<

.PHONY:
	clean

clean:
	rm -f $(OUT)