.PHONY: all, build, clean, run

all: build

build: lib.cpp
	g++ lib.cpp -o lib

run: lib
	./lib

clean:
	rm lib