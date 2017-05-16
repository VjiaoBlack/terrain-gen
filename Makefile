# Makefile
# 
# Victor Jiao
# 
# the Makefile for the project.

COMPILE=g++ -std=gnu++11

LINK=-I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image


all: main

main: build trender

build:
	mkdir build

clean: 
	rm -rf build
	rm trender


trender: build/trender.o build/TrGraphics.o
	$(COMPILE) build/*.o $(LINK) -o trender

# depends on TrGraphics.hpp for the map size - for blending, etc.
build/trender.o: src/trender.cpp src/TrGraphics.hpp
	$(COMPILE) -c src/trender.cpp $(LINK) -o build/trender.o

build/TrGraphics.o: src/TrGraphics.cpp src/TrGraphics.hpp
	$(COMPILE) -c src/TrGraphics.cpp $(LINK) -o build/TrGraphics.o

