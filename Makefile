# Makefile
# 
# Victor Jiao
# 
# the Makefile for the project.

COMPILE=g++ -std=gnu++11 -Winline -O3

LINK=-I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image

all: main

main: build trender

build:
	mkdir build

clean: 
	rm -rf build
	rm trender


trender: build/trender.o build/TrGraphics.o build/Perlin.o
	$(COMPILE) build/*.o $(LINK) -o trender

# depends on TrGraphics.hpp for the map size - for blending, etc.
build/trender.o: src/trender.cpp src/TrGraphics*
	$(COMPILE) -c src/trender.cpp $(LINK) -o build/trender.o

build/TrGraphics.o: src/TrGraphics*
	$(COMPILE) -c src/TrGraphics.cpp $(LINK) -o build/TrGraphics.o

build/Perlin.o: src/Perlin.*
	$(COMPILE) -c src/Perlin.cpp $(LINK) -o build/Perlin.o